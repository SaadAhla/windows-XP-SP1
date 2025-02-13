/*++

Copyright (c) 1989  Microsoft Corporation

Module Name:

    vcsndrcv.c

Abstract:

    This module implements all functions related to transmitting and recieving SMB's on a
    connection based transport.

Revision History:

    Balan Sethu Raman     [SethuR]    6-March-1995

Notes:


--*/

#include "precomp.h"
#pragma hdrstop

#include "vcsndrcv.h"
#include "nbtioctl.h"

//
// Forward declarations
//

NTSTATUS
VctTranceive(
    PSMBCE_SERVER_TRANSPORT pTransport,
    PSMBCEDB_SERVER_ENTRY   pServerEntry,
    PSMB_EXCHANGE           pExchange,
    ULONG                   SendOptions,
    PMDL                    pSmbMdl,
    ULONG                   SendLength,
    PVOID                   pSendCompletionContext);

NTSTATUS
VctReceive(
    PSMBCE_SERVER_TRANSPORT pTransport,
    PSMBCEDB_SERVER_ENTRY   pServerEntry,
    PSMB_EXCHANGE           pExchange);

NTSTATUS
VctSend(
    PSMBCE_SERVER_TRANSPORT pTransport,
    PSMBCEDB_SERVER_ENTRY   pServerEntry,
    ULONG                   SendOptions,
    PMDL                    pSmbMdl,
    ULONG                   SendLength,
    PVOID                   pSendCompletionContext);

NTSTATUS
VctSendDatagram(
    PSMBCE_SERVER_TRANSPORT pTransport,
    PSMBCEDB_SERVER_ENTRY   pServerEntry,
    ULONG                   SendOptions,
    PMDL                    pSmbMdl,
    ULONG                   SendLength,
    PVOID                   pSendCompletionContext);

struct _SMBCE_VC *
VctSelectVc(
    struct SMBCE_SERVER_VC_TRANSPORT *pVcTransport,
    BOOLEAN                    fMultiplexed);

NTSTATUS
VctInitializeExchange(
    PSMBCE_SERVER_TRANSPORT pTransport,
    PSMB_EXCHANGE           pExchange);

NTSTATUS
VctUninitializeExchange(
    PSMBCE_SERVER_TRANSPORT pTransport,
    PSMB_EXCHANGE           pExchange);

NTSTATUS
VctIndReceive(
    IN PVOID              pEventContext,
    IN PRXCE_VC           pVc,
    IN ULONG              ReceiveFlags,
    IN ULONG              BytesIndicated,
    IN ULONG              BytesAvailable,
    OUT ULONG             *pBytesTaken,
    IN PVOID              pTsdu,
    OUT PMDL *pDataBufferPointer,
    OUT PULONG            pDataBufferSize
    );

NTSTATUS
VctIndDataReady(
    IN PVOID        pEventContext,
    IN PMDL         pBuffer,
    IN ULONG        DataSize,
    IN NTSTATUS     CopyDataStatus
    );

NTSTATUS
VctIndEndpointError(
    IN PVOID          pEventContext,
    IN NTSTATUS       IndicatedStatus
    );

NTSTATUS
VctIndSendPossible(
    IN PVOID          pEventContext,
    IN PRXCE_VC       pRxCeVc,
    IN ULONG          BytesAvailable
    );

NTSTATUS
VctIndReceiveDatagram(
    IN PVOID   pRxCeEventContext,
    IN int     SourceAddressLength,
    IN PVOID   SourceAddress,
    IN int     OptionsLength,
    IN PVOID   Options,
    IN ULONG   ReceiveDatagramFlags,
    IN ULONG   BytesIndicated,
    IN ULONG   BytesAvailable,
    OUT ULONG  *BytesTaken,
    IN PVOID   Tsdu,
    OUT PMDL   *pDataBufferPointer,
    OUT PULONG pDataBufferSize
    );

NTSTATUS
VctIndSendComplete(
   IN PVOID          pEventContext,
   IN PRXCE_VC       pRxCeVc,
   IN PVOID          pCompletionContext,
   IN NTSTATUS       SendCompletionStatus
   );

NTSTATUS
VctCompleteInitialization(
    PSMBCEDB_SERVER_ENTRY      pServerEntry,
    PSMBCE_TRANSPORT           pTransport,
    struct SMBCE_SERVER_VC_TRANSPORT *pVcTransport);

NTSTATUS
VctUninitialize(
    PVOID pTransport);

NTSTATUS
VctpTranslateNetbiosNameToIpAddress(
    IN  OEM_STRING *pName,
    OUT ULONG      *pIpAddress
    );

ULONG
VctComputeTransportAddressSize(
   IN PUNICODE_STRING pServerName);

NTSTATUS
VctBuildTransportAddress (
    IN  PTRANSPORT_ADDRESS pTransportAddress,
    IN  ULONG              TransportAddressLength,
    IN  PUNICODE_STRING    pServerName,
    OUT PULONG             pServerIpAddress
    );

NTSTATUS
VctpCreateConnection(
    IN PSMBCEDB_SERVER_ENTRY    pServerEntry,
    IN PTRANSPORT_ADDRESS       pTransportAddress,
    IN ULONG                    TransportAddressLength,
    IN PUNICODE_STRING          pServerName,
    OUT PSMBCE_TRANSPORT        *pTransportPtr,
    IN OUT PRXCE_CONNECTION     pRxCeConnection,
    IN OUT PRXCE_VC             pRxCeVc);

VOID
VctpInitializeServerTransport(
    struct _RXCE_VC_CONNECT_CONTEXT *pRxCeConnectContext);

NTSTATUS
VctpInvokeTransportFunction(
    struct _RXCE_VC_CONNECT_CONTEXT *pRxCeConnectContext);

VOID
VctpUninitializeServerTransport(
    struct _RXCE_VC_CONNECT_CONTEXT *pRxCeConnectContext);

NTSTATUS
VctTearDownServerTransport(
   PSMBCE_SERVER_TRANSPORT pServerTransport);

NTSTATUS
VctInitiateDisconnect(
    PSMBCE_SERVER_TRANSPORT pServerTransport);

#ifdef  ALLOC_PRAGMA
#pragma alloc_text(PAGE, VctTranceive)
#pragma alloc_text(PAGE, VctReceive)
#pragma alloc_text(PAGE, VctSend)
#pragma alloc_text(PAGE, VctSendDatagram)
#pragma alloc_text(PAGE, VctSelectVc)
#pragma alloc_text(PAGE, VctInitializeExchange)
#pragma alloc_text(PAGE, VctUninitializeExchange)
#pragma alloc_text(PAGE, VctIndEndpointError)
#pragma alloc_text(PAGE, VctIndSendPossible)
#pragma alloc_text(PAGE, VctCompleteInitialization)
#pragma alloc_text(PAGE, VctUninitialize)
#pragma alloc_text(PAGE, VctpTranslateNetbiosNameToIpAddress)
#pragma alloc_text(PAGE, VctComputeTransportAddressSize)
#pragma alloc_text(PAGE, VctBuildTransportAddress)
#pragma alloc_text(PAGE, VctpCreateConnection)
#pragma alloc_text(PAGE, VctpInitializeServerTransport)
#pragma alloc_text(PAGE, VctpUninitializeServerTransport)
#pragma alloc_text(PAGE, VctpInvokeTransportFunction)
#pragma alloc_text(PAGE, VctInstantiateServerTransport)
#pragma alloc_text(PAGE, VctTearDownServerTransport)
#pragma alloc_text(PAGE, VctInitiateDisconnect)
#endif

RXDT_DefineCategory(VCSNDRCV);
#define Dbg        (DEBUG_TRACE_VCSNDRCV)

// Move this def to a common .h file.
#define MAX_SMB_PACKET_SIZE (65536)

#define MIN(a,b) ((a) < (b) ? (a) : (b))

//
// Forward references of functions ....
//

extern NTSTATUS
VctTearDownServerTransport(
    PSMBCE_SERVER_TRANSPORT pTransport);

extern NTSTATUS
VctInitializeExchange(
    PSMBCE_SERVER_TRANSPORT pTransport,
    PSMB_EXCHANGE            pExchange);

extern PSMBCE_VC
VctSelectVc(
    PSMBCE_SERVER_VC_TRANSPORT pVcTransport,
    BOOLEAN                    fMultiplexed);

extern NTSTATUS
SmbCeReceiveIndWithSecuritySignature(
    IN PSMBCEDB_SERVER_ENTRY pServerEntry,
    IN ULONG                 BytesIndicated,
    IN ULONG                 BytesAvailable,
    OUT ULONG                *pBytesTaken,
    IN PVOID                 pTsdu,
    OUT PMDL                 *pDataBufferPointer,
    OUT PULONG               pDataBufferSize,
    IN ULONG                 ReceiveFlags
    );

extern NTSTATUS
SmbCeDataReadyIndWithSecuritySignature(
   IN PSMBCEDB_SERVER_ENTRY pServerEntry,
   IN PMDL                  pBuffer,
   IN ULONG                 DataSize,
   IN NTSTATUS              CopyDataStatus);

#define SmbMmInitializeVcEntry(pVcEntry)                      \
         SmbMmInitializeHeader((pVcEntry));

#define SmbMmUninitializeVcEntry(pVcEntry)    \
         ASSERT(IsListEmpty(&(pVcEntry)->Requests.ListHead))

#define VctSelectMultiplexedVcEntry(pVcTransport)  VctSelectVc(pVcTransport,TRUE)
#define VctSelectRawVcEntry(pVcTransport)          VctSelectVc(pVcTransport,FALSE)

//
// Inline functions to update the state of a VC.
//

INLINE BOOLEAN
VctUpdateVcStateLite(
    PSMBCE_VC       pVc,
    SMBCE_VC_STATE  NewState)
{
    BOOLEAN Result = TRUE;

    ASSERT(SmbCeSpinLockAcquired());

    if (NewState == SMBCE_VC_STATE_RAW) {
        if (pVc->SwizzleCount != 0) {
            Result = FALSE;
        } else {
            pVc->State = NewState;
        }
    } else {
        pVc->State = NewState;
    }

    return Result;
}

INLINE BOOLEAN
VctUpdateVcState(
    PSMBCE_VC       pVc,
    SMBCE_VC_STATE  NewState)
{
    BOOLEAN Result = TRUE;

    SmbCeAcquireSpinLock();

    Result = VctUpdateVcStateLite(pVc,NewState);

    SmbCeReleaseSpinLock();

    return Result;
}

NTSTATUS
VctTranceive(
    PSMBCE_SERVER_TRANSPORT pTransport,
    PSMBCEDB_SERVER_ENTRY   pServerEntry,
    PSMB_EXCHANGE           pExchange,
    ULONG                   SendOptions,
    PMDL                    pSmbMdl,
    ULONG                   SendLength,
    PVOID                   pSendCompletionContext)
/*++

Routine Description:

    This routine transmits/receives a SMB for a give exchange

Arguments:

    pTransport    - the transport

    pServerEntry  - the server entry

    pExchange     - the exchange instance issuing this SMB.

    SendOptions   - options for send

    pSmbMdl       - the SMB that needs to be sent.

    SendLength    - length of data to be transmitted

    pSendCompletionContext - the send completion context

Return Value:

    STATUS_SUCCESS - the server call construction has been finalized.

    STATUS_PENDING - the open involves network traffic and the exchange has been
                     queued for notification ( pServerPointer is set to NULL)

    Other Status codes correspond to error situations.

--*/
{
    NTSTATUS                   Status = STATUS_SUCCESS;
    PSMBCE_VC                  pVc;
    PSMBCE_SERVER_VC_TRANSPORT pVcTransport;
    PSMB_HEADER                pSmbHeader = MmGetSystemAddressForMdlSafe(pSmbMdl,LowPagePriority);
    USHORT                     Mid;
    BOOLEAN                    fInvokeSendCompleteHandler = TRUE;

    PAGED_CODE();

    ASSERT(pServerEntry->Header.ObjectType == SMBCEDB_OT_SERVER);

    if (pSmbHeader == NULL) {
        Status = STATUS_INSUFFICIENT_RESOURCES;
    } else {
        pVcTransport = (PSMBCE_SERVER_VC_TRANSPORT)pTransport;

        // Ensure that the connection is still active before satisfying the request.
        if (SmbCeIsEntryInUse(&pServerEntry->Header)) {
            pVc = pExchange->SmbCeContext.TransportContext.Vcs.pVc;
            if (pVc == NULL) {
                Status = STATUS_CONNECTION_DISCONNECTED;
            }

            if ((Status == STATUS_SUCCESS) &&
                (pVc->State == SMBCE_VC_STATE_MULTIPLEXED)) {
                Status = RxCeSend(
                             &pVc->RxCeVc,
                             SendOptions,
                             pSmbMdl,
                             SendLength,
                             pSendCompletionContext);

                if ((Status == STATUS_SUCCESS) || (Status == STATUS_PENDING)) {
                    Status = STATUS_PENDING;
                    // The underlying connection engine assumes the responsibility of
                    // invoking the send complete handler from this point.
                    fInvokeSendCompleteHandler = FALSE;
                }
            } else {
                RxDbgTrace(0, Dbg, ("VctTranceive: Disconnected connection detected\n"));
                Status = STATUS_CONNECTION_DISCONNECTED;
            }
        } else {
            // The server entry is not valid ...
            Status = STATUS_CONNECTION_DISCONNECTED;
        }
    }

    if (Status != STATUS_PENDING) {
        RxDbgTrace(0, Dbg, ("VctTranceive: Return Status %lx\n",Status));
    }

    // There are instances in which the send was aborted even before the underlying
    // transport was invoked. In such cases the appropriate send complete handler
    // needs to be called so that the associated exchange can be finalized.

    if (fInvokeSendCompleteHandler) {
        NTSTATUS LocalStatus;

        LocalStatus = SmbCeSendCompleteInd(
                          pServerEntry,
                          pSendCompletionContext,
                          Status);

        RxDbgTrace(0, Dbg, ("VctTranceive: Send Complete Handler Return Status %lx\n",LocalStatus));
    }

    return Status;
}


NTSTATUS
VctReceive(
    PSMBCE_SERVER_TRANSPORT pTransport,
    PSMBCEDB_SERVER_ENTRY   pServerEntry,
    PSMB_EXCHANGE           pExchange)
/*++

Routine Description:

    This routine transmits/receives a SMB for a give exchange

Arguments:

    pTransport   - the server transport

    pServerEntry - the server entry

    pExchange  - the exchange instance issuing this SMB.

Return Value:

    STATUS_PENDING - the request has been queued

    Other Status codes correspond to error situations.

--*/
{
    NTSTATUS                   Status = STATUS_SUCCESS;
    PSMBCEDB_NET_ROOT_ENTRY    pNetRootEntry;
    PSMBCE_VC                  pVc;
    PSMBCE_SERVER_VC_TRANSPORT pVcTransport;

    PAGED_CODE();

    ASSERT(pServerEntry->Header.ObjectType == SMBCEDB_OT_SERVER);

    pVcTransport = (PSMBCE_SERVER_VC_TRANSPORT)pTransport;
    pVc          = pExchange->SmbCeContext.TransportContext.Vcs.pVc;

    // Ensure that the connection is still active before satisfying the request.
    if (SmbCeIsEntryInUse(&pServerEntry->Header) &&
        (pVc != NULL)) {
        Status = STATUS_SUCCESS;
    } else {
        // The server entry is not valid ...
        Status = STATUS_CONNECTION_DISCONNECTED;
    }

    return Status;
}

NTSTATUS
VctSend(
    PSMBCE_SERVER_TRANSPORT pTransport,
    PSMBCEDB_SERVER_ENTRY   pServerEntry,
    ULONG                   SendOptions,
    PMDL                    pSmbMdl,
    ULONG                   SendLength,
    PVOID                   pSendCompletionContext)
/*++

Routine Description:

    This routine opens/creates a server entry in the connection engine database

Arguments:

    pTransport - the server transport

    pServer    - the recepient server

    SendOptions - options for send

    pSmbMdl       - the SMB that needs to be sent.

    SendLength    - length of data to be sent

    pSendCompletionContext - the send completion context

Return Value:

    STATUS_SUCCESS - the send was successful.

    STATUS_PENDING - the send has been queued

    Other Status codes correspond to error situations.

--*/
{
    NTSTATUS                   Status = STATUS_CONNECTION_DISCONNECTED;
    PSMBCE_VC                  pVc;
    PSMBCE_SERVER_VC_TRANSPORT pVcTransport;
    BOOLEAN                    fInvokeSendCompleteHandler = TRUE;

    PAGED_CODE();

    ASSERT(pServerEntry->Header.ObjectType == SMBCEDB_OT_SERVER);

    pVcTransport = (PSMBCE_SERVER_VC_TRANSPORT)pTransport;
    pVc = VctSelectMultiplexedVcEntry(pVcTransport);

    if  (pVc != NULL) {
        if (pVc->State == SMBCE_VC_STATE_MULTIPLEXED) {
            Status = RxCeSend(
                         &pVc->RxCeVc,
                         SendOptions,
                         pSmbMdl,
                         SendLength,
                         pSendCompletionContext);

            if ((Status == STATUS_SUCCESS) || (Status == STATUS_PENDING)) {
                // The underlying connection engine assumes the responsibility of
                // invoking the send complete handler from this point.
                fInvokeSendCompleteHandler = FALSE;
            }
        }
    }

    if (!NT_SUCCESS(Status)) {
        RxDbgTrace(0, Dbg, ("VctSend: RxCeSend returned %lx\n",Status));
    }

    // There are instances in which the send was aborted even before the underlying
    // transport was invoked. In such cases the appropriate send complete handler
    // needs to be called so that the associated exchange can be finalized.

    if (fInvokeSendCompleteHandler) {
        NTSTATUS LocalStatus;

        LocalStatus = SmbCeSendCompleteInd(
                          pServerEntry,
                          pSendCompletionContext,
                          Status);

        RxDbgTrace(0, Dbg, ("VctTranceive: Send Complete Handler Return Status %lx\n",LocalStatus));
    }

    return Status;
}

NTSTATUS
VctSendDatagram(
    PSMBCE_SERVER_TRANSPORT pTransport,
    PSMBCEDB_SERVER_ENTRY   pServerEntry,
    ULONG                   SendOptions,
    PMDL                    pSmbMdl,
    ULONG                   SendLength,
    PVOID                   pSendCompletionContext)
/*++

Routine Description:

    This routine opens/creates a server entry in the connection engine database

Arguments:

    pTransport - the server transport

    pServer    - the recepient server

    SendOptions - options for send

    pSmbMdl     - the SMB that needs to be sent.

    SendLength  - length of data to be sent

    pSendCompletionContext - the send completion context

Return Value:

    STATUS_SUCCESS - the server call construction has been finalized.

    STATUS_PENDING - the open involves network traffic and the exchange has been
                     queued for notification ( pServerPointer is set to NULL)

    Other Status codes correspond to error situations.

--*/
{
    PAGED_CODE();

    return STATUS_NOT_IMPLEMENTED;
}

PSMBCE_VC
VctSelectVc(
    PSMBCE_SERVER_VC_TRANSPORT pVcTransport,
    BOOLEAN                    fMultiplexed)
/*++

Routine Description:

    This routine embodies the logic for the selection of a VC on which the SMB exchange
    will transpire

Arguments:

    pVcTransport  - the transport structure

    fMultiplexed  - the desired mode

Return Value:

    a referenced VC entry if successful otherwise NULL

--*/
{
    NTSTATUS        Status;
    PSMBCE_VC       pVc = NULL;
    ULONG           VcIndex,NumberOfActiveVcs = 0;
    SMBCE_VC_STATE  DesiredState;

    PAGED_CODE();

    if (fMultiplexed) {
        RxDbgTrace(0, Dbg, ("VctSelectVc: Referencing Multiplexed entry\n"));
        DesiredState = SMBCE_VC_STATE_MULTIPLEXED;
    } else {
        RxDbgTrace(0, Dbg, ("VctSelectVc: Referencing Raw entry\n"));
        DesiredState = SMBCE_VC_STATE_RAW;
    }

    // Acquire the resource
    SmbCeAcquireResource();

    // Choose the first VC that can support multiplexed requests
    for (VcIndex = 0; VcIndex < pVcTransport->MaximumNumberOfVCs; VcIndex++) {
        PSMBCE_VC pTempVc = &pVcTransport->Vcs[VcIndex];

        NumberOfActiveVcs++;

        if (pTempVc->State == SMBCE_VC_STATE_MULTIPLEXED) {
            if (DesiredState == SMBCE_VC_STATE_MULTIPLEXED) {
                pVc = pTempVc;
                break;
            } else {
                // If the current number of active references to a VC is zero, it can
                // be transformed into the raw mode.
                if (VctUpdateVcState(pTempVc,SMBCE_VC_STATE_RAW)) {
                    pVc = pTempVc;
                    break;
                } else {
                    NumberOfActiveVcs++;
                }
            }
        }
    }

    if (pVc == NULL) {
        // Check if it is O.K. to add VCs to this connection. Currently the server
        // implementation supports only one VC per connection. Therefore if an
        // active VC exists which has been grabbed for raw mode use an error is returned.
        // Subsequently when the server is upgraded to handle multiple VCs the logic
        // for adding a new VC will be implemented as part of this routine.
    }

    if (pVc != NULL) {
        VctReferenceVc(pVc);
    }

    // release the resource
    SmbCeReleaseResource();

    return pVc;
}

NTSTATUS
VctInitializeExchange(
    PSMBCE_SERVER_TRANSPORT pTransport,
    PSMB_EXCHANGE           pExchange)
/*++

Routine Description:

    This routine initializes the transport information pertinent to a exchange

Arguments:

    pTransport         - the transport structure

    pExchange          - the exchange instance

Return Value:

    STATUS_SUCCESS -

    Other Status codes correspond to error situations.

--*/
{
    PSMBCEDB_SERVER_ENTRY      pServerEntry;
    PSMBCE_SERVER_VC_TRANSPORT pVcTransport;

    PAGED_CODE();

    pVcTransport = (PSMBCE_SERVER_VC_TRANSPORT)pTransport;

    ASSERT(pExchange->SmbCeContext.TransportContext.Vcs.pVc == NULL);

    pExchange->SmbCeContext.TransportContext.Vcs.pVc
                     = VctSelectMultiplexedVcEntry(pVcTransport);

    if (pExchange->SmbCeContext.TransportContext.Vcs.pVc == NULL) {
        RxDbgTrace(0, Dbg, ("VctInitializeExchange: Unsuccessful\n"));
        return STATUS_CONNECTION_DISCONNECTED;
    } else {
        RxDbgTrace(0, Dbg, ("VctInitializeExchange: Successful\n"));
        return STATUS_SUCCESS;
    }
}

NTSTATUS
VctUninitializeExchange(
    PSMBCE_SERVER_TRANSPORT pTransport,
    PSMB_EXCHANGE           pExchange)
/*++

Routine Description:

    This routine uninitializes the transport information pertinent to a exchange

Arguments:

    pExchange          - the exchange instance

Return Value:

    STATUS_SUCCESS -

    Other Status codes correspond to error situations.

--*/
{
    PSMBCE_SERVER_VC_TRANSPORT pVcTransport;

    PAGED_CODE();

    pVcTransport = (PSMBCE_SERVER_VC_TRANSPORT)pTransport;

    RxDbgTrace(0, Dbg, ("VctUninitializeExchange: Successful\n"));

    if (pExchange->SmbCeContext.TransportContext.Vcs.pVc != NULL) {
        VctDereferenceVc(pExchange->SmbCeContext.TransportContext.Vcs.pVc);
    }

    pExchange->SmbCeContext.TransportContext.Vcs.pVc = NULL;

    return STATUS_SUCCESS;
}


NTSTATUS
VctIndReceive(
    IN PVOID              pEventContext,
    IN PRXCE_VC           pVc,
    IN ULONG              ReceiveFlags,
    IN ULONG              BytesIndicated,
    IN ULONG              BytesAvailable,
    OUT ULONG             *pBytesTaken,
    IN PVOID              pTsdu,                  // pointer describing this TSDU, typically a lump of bytes
    OUT PMDL              *pDataBufferPointer,    // the buffer in which data is to be copied.
    OUT PULONG            pDataBufferSize         // amount of data to copy
    )
/*++

Routine Description:

    This routine handles the receive indication for SMB's along all vcs in a connection to a
    server.

Arguments:

    pEventContext      - the server entry

    hVc                - the Vc on which the SMB has been received

    ReceiveFlags       - options for receive

    BytesIndicated     - the bytes that are present in the indication.

    BytesAvailable     - the total data available

    pTsdu              - the data

    pDataBufferPointer - the buffer for copying the data not indicated.

    pDataBufferSize    - the length of the buffer

Return Value:

    STATUS_SUCCESS -

    Other Status codes correspond to error situations.

--*/
{
    NTSTATUS Status;
    PSMBCEDB_SERVER_ENTRY  pServerEntry = (PSMBCEDB_SERVER_ENTRY)pEventContext;

    if (pServerEntry->SecuritySignaturesActive) {
        Status = SmbCeReceiveIndWithSecuritySignature(
                     pServerEntry,
                     BytesIndicated,
                     BytesAvailable,
                     pBytesTaken,
                     pTsdu,
                     pDataBufferPointer,
                     pDataBufferSize,
                     ReceiveFlags);
    } else {
        Status = SmbCeReceiveInd(
                     pServerEntry,
                     BytesIndicated,
                     BytesAvailable,
                     pBytesTaken,
                     pTsdu,
                     pDataBufferPointer,
                     pDataBufferSize,
                     ReceiveFlags);
    }


    return Status;
}

NTSTATUS
VctIndDataReady(
    IN PVOID        pEventContext,
    IN PMDL         pBuffer,
    IN ULONG        DataSize,
    IN NTSTATUS     CopyDataStatus
    )
/*++

Routine Description:

    This routine handles the indication when the requested data has been copied

Arguments:

    pEventContext - the server instance

    pBuffer       - the buffer being returned

    DataSize      - the amount of data copied in bytes

    CopyDataStatus - CopyDataStatus

Return Value:

    STATUS_SUCCESS - the server call construction has been finalized.

    Other Status codes correspond to error situations.

--*/
{
    NTSTATUS Status;
    PSMBCEDB_SERVER_ENTRY  pServerEntry = (PSMBCEDB_SERVER_ENTRY)pEventContext;

    if (pServerEntry->SecuritySignaturesActive) {
        Status = SmbCeDataReadyIndWithSecuritySignature(
                     pServerEntry,
                     pBuffer,
                     DataSize,
                     CopyDataStatus);
    } else {
        Status = SmbCeDataReadyInd(
                     pServerEntry,
                     pBuffer,
                     DataSize,
                     CopyDataStatus);
    }

    return STATUS_SUCCESS;
}

NTSTATUS
VctIndDisconnect(
    IN PVOID          pEventContext,
    IN PRXCE_VC       pRxCeVc,
    IN int            DisconnectDataLength,
    IN PVOID          DisconnectData,
    IN int            DisconnectInformationLength,
    IN PVOID          DisconnectInformation,
    IN ULONG          DisconnectFlags
    )
/*++

Routine Description:

    This routine handles the disconnect indication for a VC.

Arguments:

    pEventContext               - the server instance

    hVc                         - the virtual circuit

    DisconnectDataLength        -

    DisconnectData              -

    DisconnectInformationLength -

    DisconnectInformation       -

    DisconnectFlags             -

Return Value:

    STATUS_SUCCESS - the disconnect indication has been handled

--*/
{
    PSMBCEDB_SERVER_ENTRY       pServerEntry = (PSMBCEDB_SERVER_ENTRY)pEventContext;
    PSMBCEDB_SERVER_ENTRY       pListEntry;
    PSMBCE_VC                   pVc;
    PSMBCEDB_REQUEST_ENTRY      pRequestEntry;
    PSMB_EXCHANGE               pExchange;
    PSMBCE_SERVER_VC_TRANSPORT  pVcTransport;

    BOOLEAN fValidServerEntry = FALSE;
    BOOLEAN OutstandingWorkItem;

    // Traverse the list of server entries to ensure that the disconnect was on a
    // valid server entry. If it is not on a valid server entry ignore it.

    SmbCeAcquireSpinLock();

    pListEntry = SmbCeGetFirstServerEntry();

    while (pListEntry != NULL) {
        if (pListEntry == pServerEntry) {
            // The invalidation needs to hold onto an extra reference to avoid
            // race conditions which could lead to premature destruction of
            // this server entry.
            SmbCeReferenceServerEntry(pServerEntry);
            fValidServerEntry = TRUE;
            break;
        }
        pListEntry = SmbCeGetNextServerEntry(pListEntry);
    }
 
    if (fValidServerEntry) {
        pVcTransport = (PSMBCE_SERVER_VC_TRANSPORT)pServerEntry->pTransport;

        if (pVcTransport != NULL) {
            ULONG VcIndex;

            for (VcIndex = 0; VcIndex < pVcTransport->MaximumNumberOfVCs; VcIndex++) {
                pVc = &pVcTransport->Vcs[VcIndex];

                if (&pVc->RxCeVc == pRxCeVc) {
                    VctUpdateVcStateLite(pVc,SMBCE_VC_STATE_DISCONNECTED);
                    pVc->Status   = STATUS_CONNECTION_DISCONNECTED;
                    break;
                }
            }
        }

	OutstandingWorkItem = pServerEntry->DisconnectWorkItemOutstanding;
	// OK to unconditionally set to TRUE
	pServerEntry->DisconnectWorkItemOutstanding = TRUE;
    }

    // Release the resource
    SmbCeReleaseSpinLock();

    if (fValidServerEntry) {
        RxDbgTrace(0,Dbg,("@@@@@@ Disconnect Indication for %lx @@@@@\n",pServerEntry));
        InterlockedIncrement(&MRxSmbStatistics.ServerDisconnects);

        // Update the Server entry if this is the only VC associated with the transport.
        SmbCeTransportDisconnectIndicated(pServerEntry);
   
	// only dereference if necessary (we might already have an outstanding request)
	if(OutstandingWorkItem == FALSE ) {  

	    InitializeListHead(&pServerEntry->WorkQueueItemForDisconnect.List);

	    RxPostToWorkerThread(
		MRxSmbDeviceObject,
		CriticalWorkQueue,
		&pServerEntry->WorkQueueItemForDisconnect,
		SmbCepDereferenceServerEntry,  
		pServerEntry);
	}
	 
        RxDbgTrace(0, Dbg, ("VctIndDisconnect: Processing Disconnect indication on VC entry %lx\n",pVc));
    }

    return STATUS_SUCCESS;
}

NTSTATUS
VctIndError(
    IN PVOID          pEventContext,
    IN PRXCE_VC       pRxCeVc,
    IN NTSTATUS       IndicatedStatus
    )
/*++

Routine Description:

    This routine handles the error indication

Arguments:

    pEventContext - the server instance

    pRxCeVc       - the RxCe virtual circuit

    Status        - the error

Return Value:

    STATUS_SUCCESS

--*/
{
    NTSTATUS                   Status;
    ULONG                      VcIndex;
    PSMBCEDB_SERVER_ENTRY      pServerEntry = (PSMBCEDB_SERVER_ENTRY)pEventContext;
    PSMBCE_VC                  pVc;
    PSMBCE_SERVER_VC_TRANSPORT pVcTransport = (PSMBCE_SERVER_VC_TRANSPORT)pServerEntry->pTransport;

    // Acquire the resource
    SmbCeAcquireSpinLock();

    // Map the RXCE vc handle to the appropriate SMBCE entry and get the request
    // list associated with it.

    for (VcIndex = 0; VcIndex < pVcTransport->MaximumNumberOfVCs; VcIndex++) {
        pVc = &pVcTransport->Vcs[VcIndex];

        if (&pVc->RxCeVc == pRxCeVc) {
            VctUpdateVcStateLite(pVc,SMBCE_VC_STATE_DISCONNECTED);
            pVc->Status   = IndicatedStatus;
            break;
        }
    }

    // Release the resource
    SmbCeReleaseSpinLock();

    RxDbgTrace(0, Dbg, ("VctIndError: Processing Error indication on VC entry %lx\n",pVc));

    Status = SmbCeErrorInd(
                 pServerEntry,
                 IndicatedStatus);

    return Status;
}

NTSTATUS
VctIndEndpointError(
    IN PVOID          pEventContext,
    IN NTSTATUS       IndicatedStatus
    )
/*++

Routine Description:

    This routine handles the error indication

Arguments:

    pEventContext - the server instance

    Status        - the error

Return Value:

    STATUS_SUCCESS

--*/
{
    PAGED_CODE();

    return STATUS_SUCCESS;
}

NTSTATUS
VctIndSendPossible(
    IN PVOID          pEventContext,    // the event context.
    IN PRXCE_VC       pRxCeVc,
    IN ULONG          BytesAvailable
    )
/*++

Routine Description:

    This routine handles the error indication

Arguments:

    pEventContext - the server instance

    hVc           - the VC instance

    BytesAvailable - the number of bytes that can be sent

Return Value:

    STATUS_SUCCESS

--*/
{
    PAGED_CODE();

    return STATUS_SUCCESS;
}

NTSTATUS
VctIndReceiveDatagram(
    IN PVOID   pRxCeEventContext,      // the event context
    IN int     SourceAddressLength,    // length of the originator of the datagram
    IN PVOID   SourceAddress,          // string describing the originator of the datagram
    IN int     OptionsLength,          // options for the receive
    IN PVOID   Options,                //
    IN ULONG   ReceiveDatagramFlags,   //
    IN ULONG   BytesIndicated,         // number of bytes this indication
    IN ULONG   BytesAvailable,         // number of bytes in complete Tsdu
    OUT ULONG  *BytesTaken,            // number of bytes used
    IN PVOID   Tsdu,                   // pointer describing this TSDU, typically a lump of bytes
    OUT PMDL   *pDataBufferPointer,    // the buffer in which data is to be copied.
    OUT PULONG pDataBufferSize         // amount of data to copy
    )
{
    return STATUS_SUCCESS;
}

NTSTATUS
VctIndSendComplete(
   IN PVOID          pEventContext,
   IN PRXCE_VC       pRxCeVc,
   IN PVOID          pCompletionContext,
   IN NTSTATUS       SendCompletionStatus
   )
/*++

Routine Description:

    This routine handles the send complete indication for asynchronous sends

Arguments:

    pEventContext - the server instance

    pRxCeVc       - the RxCe VC instance

    pCompletionContext - the context for identifying the send request

    SendCompletionStatus - the send completion status

Return Value:

    STATUS_SUCCESS always ..

--*/
{
    NTSTATUS Status;

    PSMBCEDB_SERVER_ENTRY    pServerEntry = (PSMBCEDB_SERVER_ENTRY)pEventContext;

    Status = SmbCeSendCompleteInd(
                 pServerEntry,
                 pCompletionContext,
                 SendCompletionStatus);

    return Status;
}

//
// Static dispatch vectors for Virtual Circuit based transports
//

RXCE_ADDRESS_EVENT_HANDLER
MRxSmbVctAddressEventHandler = {
                                   VctIndEndpointError,
                                   VctIndReceiveDatagram,
                                   VctIndDataReady,
                                   VctIndSendPossible,
                                   NULL
                               };

RXCE_CONNECTION_EVENT_HANDLER
MRxSmbVctConnectionEventHandler = {
                                      VctIndDisconnect,
                                      VctIndError,
                                      VctIndReceive,
                                      VctIndReceiveDatagram,
                                      VctIndReceive,
                                      VctIndSendPossible,
                                      VctIndDataReady,
                                      VctIndSendComplete
                                  };

TRANSPORT_DISPATCH_VECTOR
MRxSmbVctTransportDispatch = {
                                VctSend,
                                VctSendDatagram,
                                VctTranceive,
                                VctReceive,
                                NULL,
                                VctInitializeExchange,
                                VctUninitializeExchange,
                                VctTearDownServerTransport,
                                VctInitiateDisconnect
                             };


typedef enum _RXCE_VC_FUNCTION_CODE {
   VcConnect,
   VcDisconnect
} RXCE_VC_FUNCTION_CODE, *PRXCE_VC_FUNCTION_CODE;

typedef struct _RXCE_VC_CONNECT_CONTEXT {
   RXCE_VC_FUNCTION_CODE         FunctionCode;
   PRX_WORKERTHREAD_ROUTINE      pRoutine;
   PSMBCEDB_SERVER_ENTRY         pServerEntry;
   PMRX_SRVCALL_CALLBACK_CONTEXT pCallbackContext;
   PSMBCE_SERVER_TRANSPORT       pServerTransport;
   NTSTATUS                      Status;
   KEVENT                        SyncEvent;
} RXCE_VC_CONNECT_CONTEXT, *PRXCE_VC_CONNECT_CONTEXT;

NTSTATUS
VctCompleteInitialization(
    PSMBCEDB_SERVER_ENTRY      pServerEntry,
    PSMBCE_TRANSPORT           pTransport,
    PSMBCE_SERVER_VC_TRANSPORT pVcTransport)
/*++

Routine Description:

    This routine initializes the transport information corresponding to a server

Arguments:

    pServerEntry - the server entry instance in the database

Return Value:

    STATUS_SUCCESS - the server transport construction has been finalized.

    Other Status codes correspond to error situations.

Notes:

    The remote address can be either deduced from the information in the Rx Context
    or a NETBIOS address needs to be built from the server name.
    This transport address is used subsequently to establish the connection.

--*/
{
    NTSTATUS Status;
    PSMBCE_VC                  pVc;

    RXCE_CONNECTION_INFO         ConnectionInfo;
    RXCE_TRANSPORT_PROVIDER_INFO ProviderInfo;

    PAGED_CODE();

    pVc = &pVcTransport->Vcs[0];

    // Query the transport information ...
    Status = RxCeQueryInformation(
                  &pVc->RxCeVc,
                  RxCeTransportProviderInformation,
                  &ProviderInfo,
                  sizeof(ProviderInfo));

    if (NT_SUCCESS(Status)) {
        pVcTransport->MaximumSendSize = MIN( ProviderInfo.MaxSendSize,
                                           MAXIMUM_PARTIAL_BUFFER_SIZE );
    } else {
        // CODE.IMPROVMENT - fix constant below to a #define, also is the
        //    value correct?
        ASSERT( 1024 <= MAXIMUM_PARTIAL_BUFFER_SIZE );
        pVcTransport->MaximumSendSize = 1024;
    }

    // Query the connection information ....
    Status = RxCeQueryInformation(
                 &pVc->RxCeVc,
                 RxCeConnectionEndpointInformation,
                 &ConnectionInfo,
                 sizeof(ConnectionInfo));

    if (NT_SUCCESS(Status)) {
        // The setting of the delay parameter is an important heuristic
        // that determines how quickly and how often timeouts occur. As
        // a first cut a very conservative estimate for the time has been
        // choosen, i.e., double the time required to transmit a 64 k packet.
        // This parameter should be fine tuned.

        pVcTransport->Delay.QuadPart = (-ConnectionInfo.Delay.QuadPart) +
                           (-ConnectionInfo.Delay.QuadPart);
        if (ConnectionInfo.Throughput.LowPart != 0) {
            pVcTransport->Delay.QuadPart +=
                     (MAX_SMB_PACKET_SIZE/ConnectionInfo.Throughput.LowPart) * 1000 * 10000;
        }

        RxDbgTrace( 0, Dbg, ("Connection delay set to %ld 100ns ticks\n",pVcTransport->Delay.LowPart));

        pVcTransport->pDispatchVector = &MRxSmbVctTransportDispatch;
        pVcTransport->MaximumNumberOfVCs = 1;

        pVc->State     = SMBCE_VC_STATE_MULTIPLEXED;

        pVcTransport->State = SMBCEDB_ACTIVE;
    } else {
        RxDbgTrace(0, Dbg, ("VctInitialize : RxCeQueryInformation returned %lx\n",Status));
    }

    if (NT_SUCCESS(Status)) {
        pVcTransport->pTransport   = pTransport;
    } else {
        RxDbgTrace(0, Dbg, ("VctInitialize : Connection Initialization Failed %lx\n",Status));
    }

    return Status;
}

NTSTATUS
VctUninitialize(
    PVOID pTransport)
/*++

Routine Description:

    This routine uninitializes the transport instance

Arguments:

    pVcTransport - the VC transport instance

Return Value:

    STATUS_SUCCESS - the server transport construction has been uninitialzied.

    Other Status codes correspond to error situations.

Notes:

--*/
{
    NTSTATUS                   Status = STATUS_SUCCESS;
    ULONG                      VcIndex;
    PSMBCE_VC                  pVc;
    PSMBCE_SERVER_VC_TRANSPORT pVcTransport = (PSMBCE_SERVER_VC_TRANSPORT)pTransport;
    ULONG                      TransportFlags;

    PAGED_CODE();

    // The spinlock needs to be acquired for manipulating the list of Vcs because of
    // indications that will be processed till the appropriate RXCE data structures are
    // dismantled

    for (VcIndex = 0; VcIndex < pVcTransport->MaximumNumberOfVCs; VcIndex++) {
        pVc = &pVcTransport->Vcs[VcIndex];

        // Assert the fact that the request list associated with the VC is empty.
        // Tear down the VC entry
        Status = RxCeTearDownVC(&pVc->RxCeVc);
        ASSERT(Status == STATUS_SUCCESS);
    }

    // Tear down the connection endpoint ..
    Status = RxCeTearDownConnection(&pVcTransport->RxCeConnection);
    ASSERT(Status == STATUS_SUCCESS);

    RxDbgTrace(0, Dbg, ("VctUninitialize : RxCeDisconnect returned %lx\n",Status));

    // Dereference the underlying transport
    if (pVcTransport->pTransport != NULL) {
        SmbCeDereferenceTransport(pVcTransport->pTransport);
    }

    ASSERT((pVcTransport->Vcs[0].RxCeVc.hEndpoint == INVALID_HANDLE_VALUE) ||
           (pVcTransport->Vcs[0].RxCeVc.hEndpoint == NULL));

    ASSERT(pVcTransport->Vcs[0].RxCeVc.pEndpointFileObject == NULL);

    // Free up the transport entry
    RxFreePool(pVcTransport);

    return Status;
}

NTSTATUS
VctpTranslateNetbiosNameToIpAddress(
    IN  OEM_STRING *pName,
    OUT ULONG      *pIpAddress
    )
/*++

Routine Description:

    This routine converts ascii ipaddr (11.101.4.25) into a ULONG.  This is
    based on the inet_addr code in winsock

Arguments:
    pName   - the string containing the ipaddress

Return Value:

    the ipaddress as a ULONG if it's a valid ipaddress.  Otherwise, 0.

Notes:

    The body of this routine has been borrowed fron NetBt.

--*/
{
    NTSTATUS  Status;
    PCHAR    pStr;
    int      i;
    int      len, fieldLen;
    int      fieldsDone;
    ULONG    IpAddress;
    BYTE     ByteVal;
    PCHAR    pIpPtr;
    BOOLEAN  fDotFound;
    BOOLEAN  fieldOk;

    PAGED_CODE();

    Status = STATUS_INVALID_ADDRESS_COMPONENT;

    if (pName->Length > NETBIOS_NAME_LEN) {
        return Status;
    }

    pStr = pName->Buffer;
    len = 0;
    pIpPtr = (PCHAR)&IpAddress;
    pIpPtr += 3;                   // so that we store in network order
    fieldsDone=0;

    //
    // the 11.101.4.25 format can be atmost 15 chars, and pName is guaranteed
    // to be at least 16 chars long (how convenient!!).  Convert the string to
    // a ULONG.
    //
    while(len < NETBIOS_NAME_LEN)
    {
        fieldLen=0;
        fieldOk = FALSE;
        ByteVal = 0;
        fDotFound = FALSE;

        //
        // This loop traverses each of the four fields (max len of each
        // field is 3, plus 1 for the '.'
        //
        while (fieldLen < 4)
        {
            if (*pStr >='0' && *pStr <='9')
            {
                ByteVal = (ByteVal*10) + (*pStr - '0');
                fieldOk = TRUE;
            }

            else if (*pStr == '.' || *pStr == ' ' || *pStr == '\0')
            {
                *pIpPtr = ByteVal;
                pIpPtr--;
                fieldsDone++;

                if (*pStr == '.')
                    fDotFound = TRUE;

                // if we got a space or 0, assume it's the 4th field
                if (*pStr == ' ' || *pStr == '\0')
                {
                    break;
                }
            }

            // unacceptable char: can't be ipaddr
            else
            {
                return(Status);
            }

            pStr++;
            len++;
            fieldLen++;

            // if we found the dot, we are done with this field: go to the next one
            if (fDotFound)
                break;
        }

        // this field wasn't ok (e.g. "11.101..4" or "11.101.4." etc.)
        if (!fieldOk)
        {
            return(Status);
        }

        // if we are done with all 4 fields, we are done with the outer loop too
        if ( fieldsDone == 4)
            break;

        if (!fDotFound)
        {
            return(Status);
        }
    }

    //
    // make sure the remaining chars are spaces or 0's (i.e. don't allow
    // 11.101.4.25xyz to succeed)
    //
    for (i=len; i<NETBIOS_NAME_LEN; i++, pStr++)
    {
        if (*pStr != ' ' && *pStr != '\0')
        {
            return(Status);
        }
    }

    *pIpAddress = IpAddress;
    return( STATUS_SUCCESS );
}


ULONG
VctComputeTransportAddressSize(
   IN PUNICODE_STRING pServerName)

/*++

Routine Description:

    This routine takes a computer name (PUNICODE_STRING) and computes the size of the
    TRANSPORT_ADDRESSS buffer required to connect to it.

Arguments:

    IN PUNICODE_STRING Name - Supplies the name to put into the transport

Return Value:

    size of the buffer.

Notes:

    The compound transport address passed to the transports consists of two
    TDI_NETBIOS_EX_ADDRESSes and a TDI_NETBIOS_ADDRESS. The two NETBIOS_EX addresses refer
    to the two different endpoints registered by the server, i.e., *SMBSERVER and
    the Server name padded upto NETBIOS_NAME_LEN with blanks. The order in which
    the two NETBIOS_EX addresses are constructed depend upon the length of the server
    name. If it is greater than NETBIOS_NAME_LEN *SMBSERVER is the first enpoint
    and vice versa

--*/
{
   ULONG NetbiosAddressLength,NetbiosExAddressLength,NetbiosUnicodeExAddressLength,TransportAddressSize;
   ULONG OemServerNameLength;

   PAGED_CODE();

   OemServerNameLength = RtlUnicodeStringToOemSize(pServerName);

   NetbiosAddressLength = sizeof(TDI_ADDRESS_NETBIOS);
   if( OemServerNameLength > NETBIOS_NAME_LEN ) {
       NetbiosAddressLength += OemServerNameLength - NETBIOS_NAME_LEN;
   }

   NetbiosExAddressLength = FIELD_OFFSET(TDI_ADDRESS_NETBIOS_EX,NetbiosAddress) +
                        NetbiosAddressLength;
   
   NetbiosUnicodeExAddressLength = FIELD_OFFSET(TDI_ADDRESS_NETBIOS_UNICODE_EX,RemoteNameBuffer) +
                        pServerName->Length +
                        DNS_NAME_BUFFER_LENGTH * sizeof(WCHAR);

   TransportAddressSize = FIELD_OFFSET(TRANSPORT_ADDRESS,Address) +
                          3 * FIELD_OFFSET(TA_ADDRESS,Address) +
                          NetbiosAddressLength +
                          2 * NetbiosExAddressLength +
                          NetbiosUnicodeExAddressLength;

   return TransportAddressSize;
}

NTSTATUS
VctBuildTransportAddress (
    IN  PTRANSPORT_ADDRESS pTransportAddress,
    IN  ULONG              TransportAddressLength,
    IN  PUNICODE_STRING    pServerName,
    OUT PULONG             pServerIpAddress
    )
/*++

Routine Description:

    This routine takes a computer name (PUNICODE_STRING) and converts it into an
    acceptable form for passing in as transport address.

Arguments:

    pTransportAddress      - Supplies the structure to fill in

    TransportAddressLength - Supplies the length of the buffer at TransportAddress

    pServerName            - Supplies the name to put into the transport

    pServerNameIsInIpAddressFormat = Server Name is of the dotted IP address kind

Return Value:

    None.

Notes:

    The compound transport address passed to the transports consists of two
    TDI_NETBIOS_EX_ADDRESSes and a TDI_NETBIOS_ADDRESS. The two NETBIOS_EX addresses refer
    to the two different endpoints registered by the server, i.e., *SMBSERVER and
    the Server name padded upto NETBIOS_NAME_LEN with blanks. The order in which
    the two NETBIOS_EX addresses are constructed depend upon the length of the server
    name. If it is greater than NETBIOS_NAME_LEN *SMBSERVER is the first enpoint
    and vice versa

    The WINS database can be inconsistent for extended periods of time. In order to
    account for this inconsistency on NETBIOS names and DNS names we will not
    issue the address for *SMBSERVER. This will be revisited when we have a better
    mechanism for identifying/authenticating the server and the client machine to each other.

--*/

{
    OEM_STRING OemServerName;
    NTSTATUS   Status;

    PTDI_ADDRESS_NETBIOS_EX pTdiNetbiosExAddress;
    PTDI_ADDRESS_NETBIOS    pTdiNetbiosAddress;
    PTA_ADDRESS             pFirstNetbiosExAddress,pSecondNetbiosExAddress,pNetbiosAddress,pNetbiosUnicodeExAddress;
    PTDI_ADDRESS_NETBIOS_UNICODE_EX pTdiNetbiosUnicodeExAddress;

    PCHAR  FirstEndpointName,SecondEndpointName;
    CHAR   EndpointNameBuffer[NETBIOS_NAME_LEN];
    WCHAR  UnicodeEndpointNameBuffer[NETBIOS_NAME_LEN];
    USHORT NetbiosAddressLength,NetbiosExAddressLength;
    USHORT NetbiosAddressType = TDI_ADDRESS_TYPE_NETBIOS;

    ULONG  ComponentLength;

    ULONG   RemoteIpAddress;
    BOOLEAN ServerNameIsInIpAddressForm;

    PAGED_CODE();

    if (TransportAddressLength < VctComputeTransportAddressSize(pServerName)) {
       return STATUS_BUFFER_OVERFLOW;
    }

    pFirstNetbiosExAddress = &pTransportAddress->Address[0];

    pTdiNetbiosExAddress = (PTDI_ADDRESS_NETBIOS_EX)pFirstNetbiosExAddress->Address;
    pTdiNetbiosExAddress->NetbiosAddress.NetbiosNameType = TDI_ADDRESS_NETBIOS_TYPE_QUICK_UNIQUE;

    OemServerName.Length = pServerName->Length;
    OemServerName.MaximumLength = OemServerName.Length + 1;
    OemServerName.Buffer = pTdiNetbiosExAddress->NetbiosAddress.NetbiosName;

    Status = RtlUpcaseUnicodeStringToOemString(&OemServerName, pServerName, FALSE);
    if( !NT_SUCCESS( Status ) ) {
        //return STATUS_BAD_NETWORK_PATH;
        OemServerName.Length = 0;
    }

    if (OemServerName.Length < NETBIOS_NAME_LEN) {
       RtlCopyMemory( &OemServerName.Buffer[ OemServerName.Length ],
                      "                ",
                      NETBIOS_NAME_LEN - OemServerName.Length
                    );
       OemServerName.Length = NETBIOS_NAME_LEN;
    }

    Status = VctpTranslateNetbiosNameToIpAddress(&OemServerName,&RemoteIpAddress);
    if (Status == STATUS_SUCCESS) {
        if ((RemoteIpAddress == 0) || (RemoteIpAddress == 0xffffffff)) {
           // If the server name is a valid IP address and matches with one of the two
           // broadcast addresses used by IP turn back the request.
           return STATUS_INVALID_ADDRESS_COMPONENT;
        }

        *pServerIpAddress = RemoteIpAddress;
        ServerNameIsInIpAddressForm = TRUE;
    } else {
        *pServerIpAddress = 0;
        ServerNameIsInIpAddressForm = FALSE;
    }


    NetbiosAddressLength = sizeof(TDI_ADDRESS_NETBIOS);
    if( OemServerName.Length > NETBIOS_NAME_LEN ) {
        NetbiosAddressLength += OemServerName.Length - NETBIOS_NAME_LEN;
    }

    NetbiosExAddressLength = (USHORT)(FIELD_OFFSET(TDI_ADDRESS_NETBIOS_EX,NetbiosAddress) +
                             NetbiosAddressLength);

    pFirstNetbiosExAddress->AddressLength = NetbiosExAddressLength;
    pFirstNetbiosExAddress->AddressType   = TDI_ADDRESS_TYPE_NETBIOS_EX;

#if 0
    // This arm of the code will be activated and the other arm deactivated when we have
    // mutual authenitication between server and client machines in NT5.0

    if (ServerNameIsInIpAddressForm) {
       pTransportAddress->TAAddressCount = 2;

       pNetbiosAddress = (PTA_ADDRESS)((PCHAR)pFirstNetbiosExAddress +
                                       FIELD_OFFSET(TA_ADDRESS,Address) +
                                       NetbiosExAddressLength);

       FirstEndpointName = SMBSERVER_LOCAL_ENDPOINT_NAME;
    } else {
       pTransportAddress->TAAddressCount = 3;

       pSecondNetbiosExAddress = (PTA_ADDRESS)((PCHAR)pFirstNetbiosExAddress +
                                         FIELD_OFFSET(TA_ADDRESS,Address) +
                                         NetbiosExAddressLength);

       pNetbiosAddress = (PTA_ADDRESS)((PCHAR)pSecondNetbiosExAddress +
                                       FIELD_OFFSET(TA_ADDRESS,Address) +
                                       NetbiosExAddressLength);

       // Scan the server name till the first delimiter (DNS delimiter .) and form
       // the endpoint name by padding the remaining name with blanks.

       RtlCopyMemory(
             EndpointNameBuffer,
             OemServerName.Buffer,
             NETBIOS_NAME_LEN);

       ComponentLength = 0;
       while (ComponentLength < NETBIOS_NAME_LEN) {
          if (EndpointNameBuffer[ComponentLength] == '.') {
             break;
          }
          ComponentLength++;
       }

       if (ComponentLength == NETBIOS_NAME_LEN) {
          EndpointNameBuffer[NETBIOS_NAME_LEN - 1] = ' ';
       } else {
          RtlCopyMemory(&EndpointNameBuffer[ComponentLength],
                        "                ",
                        NETBIOS_NAME_LEN - ComponentLength);
       }

       FirstEndpointName  = EndpointNameBuffer;
       SecondEndpointName = SMBSERVER_LOCAL_ENDPOINT_NAME;
    }
#else
    pTransportAddress->TAAddressCount = 3;

    pNetbiosAddress = (PTA_ADDRESS)((PCHAR)pFirstNetbiosExAddress +
                                    FIELD_OFFSET(TA_ADDRESS,Address) +
                                    NetbiosExAddressLength);
    
    if (ServerNameIsInIpAddressForm) {
       FirstEndpointName = SMBSERVER_LOCAL_ENDPOINT_NAME;
    } else {
       // Scan the server name till the first delimiter (DNS delimiter .) and form
       // the endpoint name by padding the remaining name with blanks.

       RtlCopyMemory(
             EndpointNameBuffer,
             OemServerName.Buffer,
             NETBIOS_NAME_LEN);

       ComponentLength = 0;
       while (ComponentLength < NETBIOS_NAME_LEN) {
          if (EndpointNameBuffer[ComponentLength] == '.') {
             break;
          }
          ComponentLength++;
       }

       if (ComponentLength == NETBIOS_NAME_LEN) {
          EndpointNameBuffer[NETBIOS_NAME_LEN - 1] = ' ';
       } else {
          RtlCopyMemory(&EndpointNameBuffer[ComponentLength],
                        "                ",
                        NETBIOS_NAME_LEN - ComponentLength);
       }

       FirstEndpointName  = EndpointNameBuffer;
    }
#endif

    // Copy the first endpoint name
    RtlCopyMemory(
        pTdiNetbiosExAddress->EndpointName,
        FirstEndpointName,
        NETBIOS_NAME_LEN);

#if 0
    // This will be activated alongwith the other code when mutual authentication is
    // in place
    if (!ServerNameIsInIpAddressForm) {
       // The same NETBIOS_EX address needs to be duplicated with a different endpoint name
       // for the second TA_ADDRESS.

       RtlCopyMemory(
            pSecondNetbiosExAddress,
            pFirstNetbiosExAddress,
            (FIELD_OFFSET(TA_ADDRESS,Address) + NetbiosExAddressLength));

       RtlCopyMemory(
            ((PCHAR)pSecondNetbiosExAddress +
             FIELD_OFFSET(TA_ADDRESS,Address) +
             FIELD_OFFSET(TDI_ADDRESS_NETBIOS_EX,EndpointName)),
            SecondEndpointName,
            NETBIOS_NAME_LEN);
    }
#else
    //ASSERT(pTransportAddress->TAAddressCount == 2);
#endif
    // The Netbios address associated with the first NETBIOS_EX address is the last netbios
    // address that is passed in.

    RtlCopyMemory(
         ((PCHAR)pNetbiosAddress),
         &NetbiosAddressLength,
         sizeof(USHORT));

    RtlCopyMemory(
         ((PCHAR)pNetbiosAddress + FIELD_OFFSET(TA_ADDRESS,AddressType)),
         &NetbiosAddressType,
         sizeof(USHORT));

    RtlCopyMemory(
         ((PCHAR)pNetbiosAddress + FIELD_OFFSET(TA_ADDRESS,Address)),
         &pTdiNetbiosExAddress->NetbiosAddress,
         NetbiosAddressLength);

    // Unicode Netbios name
    pNetbiosUnicodeExAddress = (PTA_ADDRESS)((PCHAR)pNetbiosAddress +
                                    FIELD_OFFSET(TA_ADDRESS,Address) +
                                    NetbiosAddressLength);

    pNetbiosUnicodeExAddress->AddressLength = (USHORT)(FIELD_OFFSET(TDI_ADDRESS_NETBIOS_UNICODE_EX,RemoteNameBuffer) +
                                              DNS_NAME_BUFFER_LENGTH * sizeof(WCHAR));
    pNetbiosUnicodeExAddress->AddressType   = TDI_ADDRESS_TYPE_NETBIOS_UNICODE_EX;
    
    pTdiNetbiosUnicodeExAddress = (PTDI_ADDRESS_NETBIOS_UNICODE_EX)pNetbiosUnicodeExAddress->Address;
    pTdiNetbiosUnicodeExAddress->NetbiosNameType = TDI_ADDRESS_NETBIOS_TYPE_QUICK_UNIQUE;
    pTdiNetbiosUnicodeExAddress->NameBufferType = NBT_READWRITE;

    pTdiNetbiosUnicodeExAddress->EndpointName.Length = (NETBIOS_NAME_LEN)*sizeof(WCHAR);
    pTdiNetbiosUnicodeExAddress->EndpointName.MaximumLength = (NETBIOS_NAME_LEN+1)*sizeof(WCHAR);
    pTdiNetbiosUnicodeExAddress->EndpointName.Buffer = (PWSTR)pTdiNetbiosUnicodeExAddress->EndpointBuffer;

    pTdiNetbiosUnicodeExAddress->RemoteName.Length = pServerName->Length;
    pTdiNetbiosUnicodeExAddress->RemoteName.MaximumLength = DNS_NAME_BUFFER_LENGTH*sizeof(WCHAR);
    pTdiNetbiosUnicodeExAddress->RemoteName.Buffer = (PWSTR)pTdiNetbiosUnicodeExAddress->RemoteNameBuffer;

    if (pTdiNetbiosUnicodeExAddress->RemoteName.MaximumLength > pServerName->Length) {
        ComponentLength = pServerName->Length;
    } else {
        ComponentLength = pTdiNetbiosUnicodeExAddress->RemoteName.MaximumLength;
    }

    RtlCopyMemory(
          pTdiNetbiosUnicodeExAddress->RemoteNameBuffer,
          pServerName->Buffer,
          ComponentLength);
    
    if (ServerNameIsInIpAddressForm) {
        RtlCopyMemory(
              pTdiNetbiosUnicodeExAddress->EndpointBuffer,
              SMBSERVER_LOCAL_ENDPOINT_NAME_UNICODE,
              NETBIOS_NAME_LEN);
    } else {
        // Scan the server name till the first delimiter (DNS delimiter .) and form
        // the endpoint name by padding the remaining name with blanks.
        
        RtlCopyMemory(
             pTdiNetbiosUnicodeExAddress->EndpointBuffer,
             L"                ",
             NETBIOS_NAME_LEN*sizeof(WCHAR));
      
        if (pTdiNetbiosUnicodeExAddress->EndpointName.Length > pServerName->Length) {
            ComponentLength = pServerName->Length;
        } else {
            ComponentLength = pTdiNetbiosUnicodeExAddress->EndpointName.Length;
        }

        RtlCopyMemory(
             pTdiNetbiosUnicodeExAddress->EndpointBuffer,
             pServerName->Buffer,
             ComponentLength);

        ComponentLength = 0;
        while (ComponentLength < NETBIOS_NAME_LEN) {
           if (pTdiNetbiosUnicodeExAddress->EndpointBuffer[ComponentLength] == L'.') {
              break;
           }
           ComponentLength++;
        }

        if (ComponentLength == NETBIOS_NAME_LEN) {
           pTdiNetbiosUnicodeExAddress->EndpointBuffer[NETBIOS_NAME_LEN - 1] = ' ';
        } else {
           RtlCopyMemory(&pTdiNetbiosUnicodeExAddress->EndpointBuffer[ComponentLength],
                        L"                ",
                        (NETBIOS_NAME_LEN-ComponentLength)*sizeof(WCHAR));
        }
    }

    //DbgPrint("Build TA %lx %lx %lx\n",pFirstNetbiosExAddress,pNetbiosAddress,pNetbiosUnicodeExAddress);
    
    return STATUS_SUCCESS;
}

typedef struct _SMBCE_VC_CONNECTION_COMPLETION_CONTEXT {
    RXCE_CONNECTION_COMPLETION_CONTEXT;

    PSMBCE_TRANSPORT_ARRAY     pTransportArray;
    PSMBCE_TRANSPORT           pTransport;
    PSMBCE_SERVER_VC_TRANSPORT pServerTransport;

    ULONG                   TransportAddressLength;
    PTRANSPORT_ADDRESS      pTransportAddress;

    PSMBCE_SERVER_TRANSPORT_CONSTRUCTION_CONTEXT pContext;
} SMBCE_VC_CONNECTION_COMPLETION_CONTEXT,
  *PSMBCE_VC_CONNECTION_COMPLETION_CONTEXT;

NTSTATUS
VctpCreateConnectionCallback(
    IN OUT PRXCE_CONNECTION_COMPLETION_CONTEXT pContext)
/*++

Routine Description:

    This is the connection callback routine initiated when the underlying
    transports have completed initialization

Arguments:

    pCOntext = the connection completion context

Notes:

--*/
{
    NTSTATUS Status;

    PSMBCE_VC_CONNECTION_COMPLETION_CONTEXT      pVcCompletionContext;
    PSMBCE_SERVER_TRANSPORT_CONSTRUCTION_CONTEXT pSmbCeContext;

    PSMBCEDB_SERVER_ENTRY pServerEntry;

    PAGED_CODE();

    ASSERT(IoGetCurrentProcess() == RxGetRDBSSProcess());

    pVcCompletionContext = (PSMBCE_VC_CONNECTION_COMPLETION_CONTEXT)pContext;
    pSmbCeContext        = pVcCompletionContext->pContext;

    pServerEntry = pSmbCeContext->pServerEntry;

    pSmbCeContext->Status = pVcCompletionContext->Status;

    Status = pVcCompletionContext->Status;

    if (Status == STATUS_SUCCESS) {
        PTA_ADDRESS pTaAdress;
        PTRANSPORT_ADDRESS pTransportAddress = (PTRANSPORT_ADDRESS)pVcCompletionContext->pConnectionInformation->RemoteAddress;
        LONG NoOfAddress;

        if (pVcCompletionContext->pTransport == NULL) {
            pVcCompletionContext->pTransport =
                pVcCompletionContext->pTransportArray->SmbCeTransports[
                    pVcCompletionContext->AddressIndex];

            SmbCeReferenceTransport(pVcCompletionContext->pTransport);
        }
        
        //DbgPrint("Remote address %lx \n",pVcCompletionContext->pConnectionInformation->RemoteAddress);

        //DbgPrint("Number of TA returned %d %lx\n",pTransportAddress->TAAddressCount,pTransportAddress->Address);
        pTaAdress = &pTransportAddress->Address[0];

        for (NoOfAddress=0; NoOfAddress<pTransportAddress->TAAddressCount;NoOfAddress++) {
            if (pTaAdress->AddressType == TDI_ADDRESS_TYPE_NETBIOS_UNICODE_EX) {
                PTDI_ADDRESS_NETBIOS_UNICODE_EX pTdiNetbiosUnicodeExAddress;

                pTdiNetbiosUnicodeExAddress = (PTDI_ADDRESS_NETBIOS_UNICODE_EX)pTaAdress->Address;
                pTdiNetbiosUnicodeExAddress->EndpointName.Buffer = (PWSTR)pTdiNetbiosUnicodeExAddress->EndpointBuffer;
                pTdiNetbiosUnicodeExAddress->RemoteName.Buffer = (PWSTR)pTdiNetbiosUnicodeExAddress->RemoteNameBuffer;
                
                SmbCeAcquireResource();
                if (pTdiNetbiosUnicodeExAddress->NameBufferType == NBT_WRITTEN) {
                    //DbgPrint("DNS name was returned from NetBT %wZ\n", &pTdiNetbiosUnicodeExAddress->RemoteName);

                    DWORD dwNewSize = pTdiNetbiosUnicodeExAddress->RemoteName.Length+2*sizeof(WCHAR);

                    // if old allocation is to small get rid of it
                    if(pServerEntry->DnsName.Buffer != NULL && 
                       dwNewSize > pServerEntry->DnsName.MaximumLength) {
                        RxFreePool(pServerEntry->DnsName.Buffer);
                        pServerEntry->DnsName.Buffer = NULL;
                    }

                    // make new allocation (if we don't already have one)
                    if(pServerEntry->DnsName.Buffer == NULL) {
                        pServerEntry->DnsName.Buffer = RxAllocatePoolWithTag(NonPagedPool, dwNewSize, MRXSMB_SERVER_POOLTAG);
                    }

                    if (pServerEntry->DnsName.Buffer != NULL) {
                        pServerEntry->DnsName.Length = pTdiNetbiosUnicodeExAddress->RemoteName.Length;
                        pServerEntry->DnsName.MaximumLength = pServerEntry->DnsName.Length+2*sizeof(WCHAR);

                        RtlCopyMemory(pServerEntry->DnsName.Buffer,
                                      pTdiNetbiosUnicodeExAddress->RemoteNameBuffer,
                                      pServerEntry->DnsName.Length);

                    } else {
                        Status = STATUS_INSUFFICIENT_RESOURCES;
                    }
                } else {
                    //DbgPrint("DNS name was not returned from NetBT for %wZ\n", &pTdiNetbiosUnicodeExAddress->RemoteName);
                    if(pServerEntry->DnsName.Buffer != NULL) {
                        RxFreePool(pServerEntry->DnsName.Buffer);
                        pServerEntry->DnsName.Buffer = NULL;
                    }
                }
                SmbCeReleaseResource();

                break;
            } else {
                //DbgPrint("TA %lx is not a NETBIOS_UNICODE_EX\n", pTaAdress);
                pTaAdress = (PTA_ADDRESS)((PCHAR)pTaAdress +
                                FIELD_OFFSET(TA_ADDRESS,Address) +
                                pTaAdress->AddressLength);
            }
        }
        
        if (Status == STATUS_SUCCESS) {
            // The Server IP address is not known. Query the underlying
            // transport for the remote transport address, i.e., NETBIOS
            // name or IP address. This will be subsequently used to
            // determine the VC number to be used in session setup and X for
            // downlevel servers.

            Status = RxCeQueryInformation(
                        pVcCompletionContext->pVc,
                        RxCeRemoteAddressInformation,
                        pVcCompletionContext->pTransportAddress,
                        pVcCompletionContext->TransportAddressLength);
        }

        if (Status == STATUS_SUCCESS) {
            ULONG   NumberOfAddresses;
            USHORT  AddressLength;
            USHORT  AddressType;
            PBYTE   pBuffer = (PBYTE)pVcCompletionContext->pTransportAddress;
            
            // All Transports currently return a data structure in which
            // the first four bytes are a ULONG which encodes the number
            // of connections opened to the given remote address. The
            // actual Transport address follows.
            pBuffer += sizeof(ULONG);

            // The buffer contains a TRANSPORT_ADDRESS, the first field
            // of which is the count.
            NumberOfAddresses = SmbGetUlong(pBuffer);

            // This is followed by an array of variable length TA_ADDRESS
            // structures. At this point pBuffer points to the first
            // TA_ADDRESS.
            pBuffer += sizeof(ULONG);

            while (NumberOfAddresses-- > 0) {
                AddressLength = SmbGetUshort(pBuffer);
                pBuffer += sizeof(USHORT);

                AddressType = SmbGetUshort(pBuffer);

                if (AddressType != TDI_ADDRESS_TYPE_IP) {
                    // skip to the next TA_ADDRESS
                    pBuffer += AddressLength + sizeof(USHORT);
                } else {
                    // Skip past the type field to position at the
                    // corresponding TDI_ADDRESS_IP structure
                    pBuffer += sizeof(USHORT);

                    // skip to the in_addr field
                    pBuffer += FIELD_OFFSET(TDI_ADDRESS_IP,in_addr);

                    // Extract the IP address
                    RtlCopyMemory(
                        &pServerEntry->Server.IpAddress,
                        pBuffer,
                        sizeof(ULONG));

                    break;
                }
            }
        } else {
           RxDbgTrace(0, Dbg, ("Remote Address Query returned %lx\n",Status));
        }

        if (NT_SUCCESS(Status)) {
            Status = VctCompleteInitialization(
                         pServerEntry,                            // The server entry
                         pVcCompletionContext->pTransport,        // the transport/address information
                         pVcCompletionContext->pServerTransport); // the server transport instance
        }

        if (NT_SUCCESS(Status)) {
            pSmbCeContext->pTransport =
                (PSMBCE_SERVER_TRANSPORT)pVcCompletionContext->pServerTransport;
            pVcCompletionContext->pServerTransport = NULL;
            pVcCompletionContext->pTransport = NULL;
        }

        pSmbCeContext->Status = Status;
    } else {
        SmbLogError(Status,
                    LOG,
                    VctpCreateConnectionCallback,
                    LOGULONG(Status)
                    LOGPTR(pServerEntry)
                    LOGUSTR(pServerEntry->Name));
    }

    if (!NT_SUCCESS(Status)) {
        RxCeTearDownVC(pVcCompletionContext->pVc);
        RxCeTearDownConnection(pVcCompletionContext->pConnection);
        SmbCeDereferenceTransport(pVcCompletionContext->pTransport);
        pVcCompletionContext->pTransport = NULL;
    }

    if (pVcCompletionContext->pTransportArray != NULL) {
        SmbCeDereferenceTransportArray(pVcCompletionContext->pTransportArray);
    }

    if (pVcCompletionContext->pTransportAddress != NULL) {
        RxFreePool(pVcCompletionContext->pTransportAddress);
    }

    if (pVcCompletionContext->pConnectionInformation != NULL) {
        RxFreePool(pVcCompletionContext->pConnectionInformation);
    }

    ASSERT(pVcCompletionContext->pTransport == NULL);

    if (pVcCompletionContext->pServerTransport != NULL) {
        SmbMmFreeServerTransport(
            (PSMBCE_SERVER_TRANSPORT)pVcCompletionContext->pServerTransport);
    }

    RxFreePool(pVcCompletionContext);

    pSmbCeContext->State  = SmbCeServerVcTransportConstructionEnd;

    SmbCeConstructServerTransport(pSmbCeContext);
    return STATUS_SUCCESS;
}

NTSTATUS
VctInstantiateServerTransport(
    IN OUT PSMBCE_SERVER_TRANSPORT_CONSTRUCTION_CONTEXT pContext)
/*++

Routine Description:

    This routine initializes the transport information corresponding to a server

Arguments:

    pContext - the transport construction context

Return Value:

    STATUS_PENDING - asynchronous construction has been initiated

Notes:

    Currently, only connection oriented transports are handled. The current TDI
    spec expects handles to be passed in as part of the connect request. This
    implies that connect/reconnect/disconnect requests need to be issued from the
    process which created the connection. In the case of the SMB mini rdr there
    is no FSP associated with it ( threads are borrowed/commandeered ) from the
    system process to do all the work. This is the reason for special casing VC
    initialization into a separate routine. The server transport initialization
    routine handles the other transport initialization and also provides the
    context for VC initialization.

--*/
{
    NTSTATUS Status = STATUS_PENDING;

    PSMBCE_TRANSPORT_ARRAY pTransportArray;

    PAGED_CODE();

    ASSERT(IoGetCurrentProcess() == RxGetRDBSSProcess());

    pTransportArray   = SmbCeReferenceTransportArray();


    if (pTransportArray == NULL) {
        Status = STATUS_NETWORK_UNREACHABLE;
    } else {
        PSMBCEDB_SERVER_ENTRY  pServerEntry;
        UNICODE_STRING         ServerName;

        PSMBCE_VC_CONNECTION_COMPLETION_CONTEXT pCompletionContext;
        PRXCE_CONNECTION_INFORMATION InitialConnectionInformation = NULL;

        ULONG ServerIpAddress;

        pServerEntry = pContext->pServerEntry;

        ServerName.Buffer        = pServerEntry->Name.Buffer + 1;
        ServerName.Length        = pServerEntry->Name.Length - sizeof(WCHAR);
        ServerName.MaximumLength = pServerEntry->Name.MaximumLength - sizeof(WCHAR);

        pServerEntry->Server.IpAddress = 0;

        pCompletionContext = (PSMBCE_VC_CONNECTION_COMPLETION_CONTEXT)
                             RxAllocatePoolWithTag(
                                 NonPagedPool,
                                 sizeof(SMBCE_VC_CONNECTION_COMPLETION_CONTEXT),
                                 MRXSMB_VC_POOLTAG);

        if (pCompletionContext != NULL) {
            RtlZeroMemory(pCompletionContext,sizeof(SMBCE_VC_CONNECTION_COMPLETION_CONTEXT));
            
            pCompletionContext->pContext = pContext;

            pCompletionContext->TransportAddressLength = VctComputeTransportAddressSize(
                                                             &ServerName);

            pCompletionContext->pTransportAddress = (PTRANSPORT_ADDRESS)
                                                    RxAllocatePoolWithTag(
                                                        NonPagedPool,
                                                        pCompletionContext->TransportAddressLength,
                                                        MRXSMB_VC_POOLTAG);

            if (pCompletionContext->pTransportAddress == NULL) {
                Status = STATUS_INSUFFICIENT_RESOURCES;
            } else {
                RtlZeroMemory(pCompletionContext->pTransportAddress,
                              pCompletionContext->TransportAddressLength);

                Status = VctBuildTransportAddress(
                             pCompletionContext->pTransportAddress,
                             pCompletionContext->TransportAddressLength,
                             &ServerName,
                             &ServerIpAddress);
            }

            if (Status == STATUS_SUCCESS) {
                pCompletionContext->pServerTransport = (PSMBCE_SERVER_VC_TRANSPORT)
                                                       SmbMmAllocateServerTransport(
                                                           SMBCE_STT_VC);

                if (pCompletionContext->pServerTransport == NULL) {
                    Status = STATUS_INSUFFICIENT_RESOURCES;
                } else {
                    pCompletionContext->pConnection =
                        &(pCompletionContext->pServerTransport->RxCeConnection);
                    pCompletionContext->pVc =
                        &(pCompletionContext->pServerTransport->Vcs[0].RxCeVc);
                }
            }

            if (Status == STATUS_SUCCESS) {
                InitialConnectionInformation = RxAllocatePoolWithTag(
                                                   NonPagedPool,
                                                   sizeof(RXCE_CONNECTION_INFORMATION),
                                                   MRXSMB_VC_POOLTAG);

                if (InitialConnectionInformation == NULL) {
                    Status = STATUS_INSUFFICIENT_RESOURCES;
                } else {
                    InitialConnectionInformation->UserDataLength = 0;
                    InitialConnectionInformation->OptionsLength  = 0;
                    InitialConnectionInformation->RemoteAddressLength = pCompletionContext->TransportAddressLength;
                    InitialConnectionInformation->RemoteAddress = pCompletionContext->pTransportAddress;
                }
            }

            if (Status == STATUS_SUCCESS) {
                PSMBCE_TRANSPORT        pTransport;
                
                pCompletionContext->pTransport = NULL;
                pCompletionContext->pTransportArray   = pTransportArray;
                pCompletionContext->pConnectionInformation = InitialConnectionInformation;
                //DbgPrint("Remote address %lx \n",pCompletionContext->pConnectionInformation->RemoteAddress);

                if (pServerEntry->PreferredTransport != NULL) {
                    pTransport = pServerEntry->PreferredTransport;

                    Status = RxCeBuildConnection(
                                 &pTransport->RxCeAddress,
                                 InitialConnectionInformation,
                                 &MRxSmbVctConnectionEventHandler,
                                 pServerEntry,
                                 pCompletionContext->pConnection,
                                 pCompletionContext->pVc);

                    if (Status == STATUS_SUCCESS) {
                        pCompletionContext->pTransport = pTransport;
                        SmbCeReferenceTransport(pTransport);
                    }

                    ASSERT(Status != STATUS_PENDING);

                    if (Status != STATUS_SUCCESS) {
                        SmbCeDereferenceTransport(pServerEntry->PreferredTransport);
                        pServerEntry->PreferredTransport = NULL;
                    }

                    pCompletionContext->Status = Status;

                    VctpCreateConnectionCallback(
                        (PRXCE_CONNECTION_COMPLETION_CONTEXT)pCompletionContext);

                    Status = STATUS_PENDING;
                } else {

                    Status = RxCeBuildConnectionOverMultipleTransports(
                                 MRxSmbDeviceObject,
                                 MRxSmbObeyBindingOrder ?
                                    RxCeSelectBestSuccessfulTransport :
                                    RxCeSelectFirstSuccessfulTransport,

                                 pCompletionContext->pTransportArray->Count,
                                 pCompletionContext->pTransportArray->LocalAddresses,
                                 &ServerName,
                                 InitialConnectionInformation,
                                 &MRxSmbVctConnectionEventHandler,
                                 pServerEntry,
                                 VctpCreateConnectionCallback,
                                 (PRXCE_CONNECTION_COMPLETION_CONTEXT)pCompletionContext);

//                    ASSERT(Status == STATUS_PENDING);
                }
            }
        } else {
            Status = STATUS_INSUFFICIENT_RESOURCES;
        }

        if (Status != STATUS_PENDING) {
            if (pCompletionContext != NULL) {
                if (pCompletionContext->pTransportAddress != NULL) {
                    RxFreePool(pCompletionContext->pTransportAddress);
                }

                if (pCompletionContext->pServerTransport != NULL) {
                    RxFreePool(pCompletionContext->pServerTransport);
                }
                
                RxFreePool(pCompletionContext);
            }

            if (InitialConnectionInformation != NULL) {
                RxFreePool(InitialConnectionInformation);
            }

            SmbCeDereferenceTransportArray(pTransportArray);
        }
    }

    if (Status != STATUS_PENDING) {
        ASSERT(Status != STATUS_SUCCESS);

        pContext->State  = SmbCeServerVcTransportConstructionEnd;
        pContext->Status = Status;

        // Call the construct server transport routine to complete the construction
        SmbCeConstructServerTransport(pContext);

        Status = STATUS_PENDING;
    }

    return Status;
}

NTSTATUS
VctTearDownServerTransport(
   PSMBCE_SERVER_TRANSPORT pServerTransport)
{
    NTSTATUS Status;
    PKEVENT pRundownEvent = pServerTransport->pRundownEvent;

    PAGED_CODE();

    ASSERT(IoGetCurrentProcess() == RxGetRDBSSProcess());

    Status = VctUninitialize(pServerTransport);

    if (pRundownEvent != NULL) {
        KeSetEvent(pRundownEvent, 0, FALSE );
    }

    return Status;
}

NTSTATUS
VctInitiateDisconnect(
    PSMBCE_SERVER_TRANSPORT pServerTransport)
{
    ULONG                      VcIndex;
    PSMBCE_VC                  pVc;
    PSMBCE_SERVER_VC_TRANSPORT pVcTransport = (PSMBCE_SERVER_VC_TRANSPORT)pServerTransport;

    ASSERT(IoGetCurrentProcess() == RxGetRDBSSProcess());

    for (VcIndex = 0; VcIndex < pVcTransport->MaximumNumberOfVCs; VcIndex++) {
        NTSTATUS Status;

        pVc = &pVcTransport->Vcs[VcIndex];

        Status = RxCeInitiateVCDisconnect(&pVc->RxCeVc);

        if (Status != STATUS_SUCCESS) {
            RxDbgTrace(0, Dbg, ("VctInitiateDisconnect: Disconnected Status %lxd\n",Status));
        }
    }

    return STATUS_SUCCESS;
}

PFILE_OBJECT
SmbCepReferenceEndpointFileObject(
    PSMBCE_SERVER_TRANSPORT pTransport)
/*++

Routine Description:

    This routine returns the connection file object associated with
    a transport

Arguments:

    pTransport - the transport instance

Notes:

    This routine currently returns this for VC transports. When we implement
    other transports a suitable abstraction needs to be implemented

--*/
{
    PFILE_OBJECT         pEndpointFileObject =  NULL;
    PSMBCE_OBJECT_HEADER pHeader = (PSMBCE_OBJECT_HEADER)pTransport;

    if ((pHeader != NULL) && (pHeader->ObjectType == SMBCE_STT_VC)) {
        PSMBCE_SERVER_VC_TRANSPORT pVcTransport = (PSMBCE_SERVER_VC_TRANSPORT)pTransport;

        pEndpointFileObject = pVcTransport->Vcs[0].RxCeVc.pEndpointFileObject;

        if (pEndpointFileObject != NULL) {
            ObReferenceObject(pEndpointFileObject);
        }

    }

    return pEndpointFileObject;
}
