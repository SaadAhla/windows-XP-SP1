/*++

Copyright (c) 1995  Microsoft Corporation

Module Name:
	mqattrib.h

Abstract:
	MSMQ classes and attributes in NT5
	
Author:

    Ronit Hartmann (ronith)

--*/
#ifndef __mqattrib_h__
#define __mqattrib_h__

//-----------------------------------------------------
//      MSMQ-service
//-----------------------------------------------------
const WCHAR MSMQ_SERVICE_CLASS_NAME[] = L"mSMQEnterpriseSettings";

const WCHAR x_ServiceCategoryName[] = L"CN=MSMQ-Enterprise-Settings";
const DWORD x_ServiceCategoryLength = (sizeof( x_ServiceCategoryName)/sizeof(WCHAR)) - 1;

const WCHAR MQ_E_NAMESTYLE_ATTRIBUTE[] = L"mSMQNameStyle"; // PROPID_E_NAMESTYLE
const WCHAR MQ_E_CSP_NAME_ATTRIBUTE[] = L"mSMQCSPName";    // PROPID_E_CSP_NAME
const WCHAR MQ_E_ID_ATTRIBUTE[] = L"objectGUID";           // PROPID_E_ID
const WCHAR MQ_E_LONG_LIVE_ATTRIBUTE[] = L"mSMQLongLived";  // PROPID_E_LONG_LIVE
const WCHAR MQ_E_VERSION_ATTRIBUTE[] = L"mSMQVersion";     // PROPID_E_VERSION
const WCHAR MQ_E_SECURITY_ATTRIBUTE[] = L"nTSecurityDescriptor"; // PROPID_E_SECURITY
const WCHAR MQ_E_NT4ID_ATTRIBUTE[] = L"objectGUID";        // PROPID_E_NT4ID
const WCHAR MQ_E_INTERVAL1[] = L"mSMQInterval1";           //PROPID_E_S_INTERVAL1
const WCHAR MQ_E_INTERVAL2[] = L"mSMQInterval2";           //PROPID_E_S_INTERVAL2


const ADSTYPE MQ_E_NAMESTYLE_ADSTYPE = ADSTYPE_BOOLEAN;
const ADSTYPE MQ_E_CSP_NAME_ADSTYPE = ADSTYPE_CASE_IGNORE_STRING;
const ADSTYPE MQ_E_ID_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_E_LONG_LIVE_ADSTYPE = ADSTYPE_INTEGER;
const ADSTYPE MQ_E_VERSION_ADSTYPE = ADSTYPE_INTEGER;
const ADSTYPE MQ_E_SECURITY_ADSTYPE = ADSTYPE_NT_SECURITY_DESCRIPTOR;
const ADSTYPE MQ_E_NT4ID_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_E_INTERVAL1_ADSTYPE = ADSTYPE_INTEGER;
const ADSTYPE MQ_E_INTERVAL2_ADSTYPE = ADSTYPE_INTEGER;

//-----------------------------------------------------
//      MSMQ-site-link
//-----------------------------------------------------
const WCHAR MSMQ_SITELINK_CLASS_NAME[] = L"mSMQSiteLink";

const WCHAR x_LinkCategoryName[] = L"CN=MSMQ-Site-Link";
const DWORD x_LinkCategoryLength = (sizeof( x_LinkCategoryName)/sizeof(WCHAR)) - 1;

const WCHAR MQ_L_NEIGHBOR1_ATTRIBUTE[] = L"mSMQSite1";    // PROPID_L_NEIGHBOR1
const WCHAR MQ_L_NEIGHBOR2_ATTRIBUTE[] = L"mSMQSite2";    // PROPID_L_NEIGHBOR2
const WCHAR MQ_L_COST_ATTRIBUTE[] = L"mSMQCost";         // PROPID_L_COST
const WCHAR MQ_L_ID_ATTRIBUTE[] = L"objectGUID";           // PROPID_L_ID
const WCHAR MQ_L_SITEGATES_ATTRIBUTE[] = L"mSMQSiteGates";        // PROPID_S_GATES
const WCHAR MQ_L_DESCRIPTION_ATTRIBUTE[] = L"description";  // PROPID_L_DESCRIPTION
const WCHAR MQ_L_FULL_PATH_ATTRIBUTE[] = L"distinguishedName"; // PROPID_L_FULL_PATH

const ADSTYPE MQ_L_NEIGHBOR1_ADSTYPE = ADSTYPE_DN_STRING;
const ADSTYPE MQ_L_NEIGHBOR2_ADSTYPE = ADSTYPE_DN_STRING;
const ADSTYPE MQ_L_COST_ADSTYPE = ADSTYPE_INTEGER;
const ADSTYPE MQ_L_ID_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_L_SITEGATES_ADSTYPE = ADSTYPE_DN_STRING;
const ADSTYPE MQ_L_DESCRIPTION_ADSTYPE = ADSTYPE_CASE_IGNORE_STRING;
const ADSTYPE MQ_L_FULL_PATH_ADSTYPE = ADSTYPE_DN_STRING;
//-----------------------------------------------------
//      User
//-----------------------------------------------------
const WCHAR MSMQ_USER_CLASS_NAME[] = L"user";

const WCHAR x_UserCategoryName[] = L"CN=Person";
const DWORD x_UserCategoryLength = (sizeof( x_UserCategoryName)/sizeof(WCHAR)) - 1;

const WCHAR MQ_U_SID_ATTRIBUTE[] = L"objectSid";           // PROPID_U_SID
const WCHAR MQ_U_SIGN_CERT_ATTRIBUTE[] = L"mSMQSignCertificates";        // PROPID_U_SIGN_CERT
const WCHAR MQ_U_DIGEST_ATTRIBUTE[] = L"mSMQDigests";      // PROPID_U_DIGEST
const WCHAR MQ_U_ID_ATTRIBUTE[] = L"objectGUID";          // PROPID_U_ID

const ADSTYPE MQ_U_SID_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_U_SIGN_CERT_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_U_DIGEST_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_U_ID_ADSTYPE = ADSTYPE_OCTET_STRING;

//-----------------------------------------------------
//      MQUser
//-----------------------------------------------------
#define MSMQ_OBJECT_SID_ATTRIBUTE   (L"mSMQUserSid")
#define MSMQ_OBJECT_SID_DESCRIPTION (L"MSMQ-User-sid")

const WCHAR MSMQ_MQUSER_CLASS_NAME[] = L"mSMQMigratedUser";

const WCHAR x_MQUserCategoryName[] = L"CN=MSMQ-Migrated-User";
const DWORD x_MQUserCategoryLength = (sizeof( x_MQUserCategoryName)/sizeof(WCHAR)) - 1;


const WCHAR MQ_MQU_SID_ATTRIBUTE[] = MSMQ_OBJECT_SID_ATTRIBUTE;        // PROPID_MQU_SID
const WCHAR MQ_MQU_SIGN_CERT_ATTRIBUTE[] = L"mSMQSignCertificates";    // PROPID_MQU_SIGN_CERT
const WCHAR MQ_MQU_DIGEST_ATTRIBUTE[] = L"mSMQDigests";                // PROPID_MQU_DIGEST
const WCHAR MQ_MQU_ID_ATTRIBUTE[] = L"objectGUID";                     // PROPID_MQU_ID
const WCHAR MQ_MQU_SECURITY_ATTRIBUTE[] = L"nTSecurityDescriptor";     // PROPID_MQU_SECURITY

const ADSTYPE MQ_MQU_SID_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_MQU_SIGN_CERT_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_MQU_DIGEST_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_MQU_ID_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_MQU_SECURITY_ADSTYPE = ADSTYPE_NT_SECURITY_DESCRIPTOR;

//-----------------------------------------------------
//      MSMQ-computer-configuration
//-----------------------------------------------------
const WCHAR MSMQ_COMPUTER_CONFIGURATION_CLASS_NAME[] = L"mSMQConfiguration";

const WCHAR x_ComputerConfigurationCategoryName[] = L"CN=MSMQ-Configuration";
const DWORD x_ComputerConfigurationCategoryLength = (sizeof( x_ComputerConfigurationCategoryName)/sizeof(WCHAR)) - 1;

const WCHAR MQ_QM_ID_ATTRIBUTE[] = L"objectGUID";          // PROPID_QM_MACHINE_ID
const WCHAR MQ_QM_OUTFRS_ATTRIBUTE[] = L"mSMQOutRoutingServers";  // PROPID_QM_OUTFRS_DN
const WCHAR MQ_QM_INFRS_ATTRIBUTE[] = L"mSMQInRoutingServers";   // PROPID_QM_INFRS_DN
const WCHAR MQ_QM_SERVICE_ATTRIBUTE[] = L"mSMQServiceType"; // PROPID_QM_SERVICE  // [adsrv]
const WCHAR MQ_QM_SERVICE_ROUTING_ATTRIBUTE[]   = L"mSMQRoutingServices";         // PROPID_QM_SERVICE_ROUTING
const WCHAR MQ_QM_SERVICE_DSSERVER_ATTRIBUTE[]  = L"mSMQDsServices";              // PROPID_QM_SERVICE_DSSERVER
const WCHAR MQ_QM_SERVICE_DEPCLIENTS_ATTRIBUTE[]= L"mSMQDependentClientServices"; // PROPID_QM_SERVICE_DEPCLIENTS
const WCHAR MQ_QM_QUOTA_ATTRIBUTE[] = L"mSMQQuota";   // PROPID_QM_QUOTA
const WCHAR MQ_QM_JOURNAL_QUOTA_ATTRIBUTE[] = L"mSMQJournalQuota"; // PROPID_QM_JOURNAL_QUOTA
const WCHAR MQ_QM_MACHINE_TYPE_ATTRIBUTE[] = L"mSMQComputerTypeEx"; // PROPID_QM_MACHINE
const WCHAR MQ_QM_FOREIGN_ATTRIBUTE[] = L"mSMQForeign"; // PROPID_QM_FOREIGN
const WCHAR MQ_QM_OS_ATTRIBUTE[] = L"mSMQOSType";      // PROPID_QM_OS
const WCHAR MQ_QM_SECURITY_ATTRIBUTE[] = L"nTSecurityDescriptor"; // PROPID_QM_SECURITY
const WCHAR MQ_QM_SIGN_PK_ATTRIBUTE[] = L"mSMQSignKey";          // PROPID_QM_SIGN_PK
const WCHAR MQ_QM_ENCRYPT_PK_ATTRIBUTE[] = L"mSMQEncryptKey";     // PROPID_QM_ENCRYPT_PK
const WCHAR MQ_QM_CREATE_TIME_ATTRIBUTE[] = L"whenCreated";         // PROPID_QM_CREATE_TIME
const WCHAR MQ_QM_MODIFY_TIME_ATTRIBUTE[] = L"whenChanged";         // PROPID_QM_MODIFY_TIME
const WCHAR MQ_QM_SITES_ATTRIBUTE[] = L"mSMQSites";   // PROPID_QM_SITE_IDS
const WCHAR MQ_QM_PATHNAME_ATTRIBUTE[] = L"cn";                  // PROPID_QM_PATHNAME
const WCHAR MQ_QM_FULL_PATH_ATTRIBUTE[] = L"distinguishedName";   //PROPID_QM_FULL_PATH
const WCHAR MQ_QM_MASTERID_ATTRIBUTE[] = L"mSMQOwnerID";          //PROPID_QM_MASTERID
const WCHAR MQ_QM_NT4ID_ATTRIBUTE[] = L"objectGUID";              //PROPID_QM_NT4ID
const WCHAR MQ_QM_WORKGROUP_ID_ATTRIBUTE[] = L"objectGUID";       //PROPID_QM_WORKGROUP_ID
const WCHAR MQ_QM_DESCRIPTION_ATTRIBUTE[] = L"description";       //PROPID_QM_DESCRIPTION

const ADSTYPE MQ_QM_ID_ADSTYPE  = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_QM_OUTFRS_ADSTYPE = ADSTYPE_DN_STRING;
const ADSTYPE MQ_QM_INFRS_ADSTYPE = ADSTYPE_DN_STRING;
const ADSTYPE MQ_QM_SERVICE_ADSTYPE = ADSTYPE_INTEGER;  // [adsrv]
const ADSTYPE MQ_QM_SERVICE_ROUTING_ADSTYPE = ADSTYPE_BOOLEAN;
const ADSTYPE MQ_QM_SERVICE_DSSERVER_ADSTYPE = ADSTYPE_BOOLEAN;
const ADSTYPE MQ_QM_SERVICE_DEPCLIENTS_ADSTYPE = ADSTYPE_BOOLEAN;
const ADSTYPE MQ_QM_QUOTA_ADSTYPE = ADSTYPE_INTEGER;
const ADSTYPE MQ_QM_JOURNAL_QUOTA_ADSTYPE = ADSTYPE_INTEGER;
const ADSTYPE MQ_QM_MACHINE_TYPE_ADSTYPE = ADSTYPE_CASE_IGNORE_STRING;
const ADSTYPE MQ_QM_FOREIGN_ADSTYPE = ADSTYPE_BOOLEAN;
const ADSTYPE MQ_QM_OS_ADSTYPE = ADSTYPE_INTEGER;
const ADSTYPE MQ_QM_SECURITY_ADSTYPE = ADSTYPE_NT_SECURITY_DESCRIPTOR;
const ADSTYPE MQ_QM_SIGN_PK_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_QM_ENCRYPT_PK_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_QM_CREATE_TIME_ADSTYPE = ADSTYPE_UTC_TIME;
const ADSTYPE MQ_QM_MODIFY_TIME_ADSTYPE = ADSTYPE_UTC_TIME;
const ADSTYPE MQ_QM_SITES_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_QM_FULL_PATH_ADSTYPE = ADSTYPE_DN_STRING;
const ADSTYPE MQ_QM_MASTERID_ADSTYPE  = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_QM_NT4ID_ADSTYPE  = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_QM_WORKGROUP_ID_ADSTYPE  = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_QM_DESCRIPTION_ADSTYPE = ADSTYPE_CASE_IGNORE_STRING; 

//-----------------------------------------------------
//      MSMQ-queue

// TOP,  Superior computer
//-----------------------------------------------------
const WCHAR MSMQ_QUEUE_CLASS_NAME[] = L"mSMQQueue";

const WCHAR x_QueueCategoryName[] = L"CN=MSMQ-Queue";
const DWORD x_QueueCategoryLength = (sizeof( x_QueueCategoryName)/sizeof(WCHAR)) - 1;

const WCHAR MQ_Q_INSTANCE_ATTRIBUTE[] = L"objectGUID";          // PROPIQ_Q_INSTANCE
const WCHAR MQ_Q_TYPE_ATTRIBUTE[] = L"mSMQQueueType";      // PROPID_Q_TYPE
const WCHAR MQ_Q_PATHNAME_ATTRIBUTE[] = L"distinguishedName";     // PROPID_Q_PATHNAME
const WCHAR MQ_Q_JOURNAL_ATTRIBUTE[] = L"mSMQJournal";    // PROPID_Q_JOURNAL
const WCHAR MQ_Q_QUOTA_ATTRIBUTE[] = L"mSMQQueueQuota";     // PROPID_Q_QUOTA
const WCHAR MQ_Q_BASEPRIORITY_ATTRIBUTE[] = L"mSMQBasePriority";  // PROPID_Q_BASEPRIORITY
const WCHAR MQ_Q_JOURNAL_QUOTA_ATTRIBUTE[] = L"mSMQQueueJournalQuota";  // PROPID_Q_JOURNALQUOTA
const WCHAR MQ_Q_LABEL_ATTRIBUTE[] = L"mSMQLabelEx";         // PROPID_Q_LABEL
const WCHAR MQ_Q_CREATE_TIME_ATTRIBUTE[] = L"whenCreated";         // PROPID_Q_CREATE_TIME
const WCHAR MQ_Q_MODIFY_TIME_ATTRIBUTE[] = L"whenChanged";         // PROPID_Q_MODIFY_TIME
const WCHAR MQ_Q_AUTHENTICATE_ATTRIBUTE[] = L"mSMQAuthenticate";  // PROPID_Q_AUTHENTICATE
const WCHAR MQ_Q_PRIV_LEVEL_ATTRIBUTE[] = L"mSMQPrivacyLevel";  // PROPID_Q_PRIV_LEVEL
const WCHAR MQ_Q_TRANSACTION_ATTRIBUTE[] = L"mSMQTransactional";     // PROPID_Q_TRANSACTION
const WCHAR MQ_Q_SECURITY_ATTRIBUTE[] = L"nTSecurityDescriptor";     // PROPID_Q_SECURITY
const WCHAR MQ_Q_MASTERID_ATTRIBUTE[] = L"mSMQOwnerID";     // PROPID_Q_MASTERID
const WCHAR MQ_Q_NT4ID_ATTRIBUTE[] = L"objectGUID";          // PROPIQ_Q_NT4ID
const WCHAR MQ_Q_FULL_PATH_ATTRIBUTE[] = L"distinguishedName"; // PROPID_Q_FULL_PATH
const WCHAR MQ_Q_NAME_EXT[] = L"mSMQQueueNameExt";
const WCHAR MQ_Q_MULTICAST_ADDRESS[] = L"MSMQ-MulticastAddress"; // PROPID_Q_MULTICAST_ADDRESS

const ADSTYPE MQ_Q_INSTANCE_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_Q_TYPE_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_Q_JOURNAL_ADSTYPE = ADSTYPE_BOOLEAN;
const ADSTYPE MQ_Q_QUOTA_ADSTYPE = ADSTYPE_INTEGER;
const ADSTYPE MQ_Q_BASEPRIORITY_ADSTYPE = ADSTYPE_INTEGER;
const ADSTYPE MQ_Q_JOURNAL_QUOTA_ADSTYPE = ADSTYPE_INTEGER;
const ADSTYPE MQ_Q_LABEL_ADSTYPE = ADSTYPE_CASE_IGNORE_STRING;
const ADSTYPE MQ_Q_CREATE_TIME_ADSTYPE = ADSTYPE_UTC_TIME;
const ADSTYPE MQ_Q_MODIFY_TIME_ADSTYPE = ADSTYPE_UTC_TIME;
const ADSTYPE MQ_Q_AUTHENTICATE_ADSTYPE = ADSTYPE_BOOLEAN;
const ADSTYPE MQ_Q_PRIV_LEVEL_ADSTYPE = ADSTYPE_INTEGER;
const ADSTYPE MQ_Q_TRANSACTION_ADSTYPE = ADSTYPE_BOOLEAN;
const ADSTYPE MQ_Q_SECURITY_ADSTYPE = ADSTYPE_NT_SECURITY_DESCRIPTOR;
const ADSTYPE MQ_Q_MASTERID_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_Q_NT4ID_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_Q_FULL_PATH_ADSTYPE = ADSTYPE_DN_STRING;
const ADSTYPE MQ_Q_NAME_EXT_ADSTYPE = ADSTYPE_CASE_EXACT_STRING;
const ADSTYPE MQ_Q_MULTICAST_ADDRESS_ADSTYPE = ADSTYPE_CASE_IGNORE_STRING;


//-----------------------------------------------------
//      Site
//-----------------------------------------------------
const WCHAR MSMQ_SITE_CLASS_NAME[] = L"site";

const WCHAR x_SiteCategoryName[] = L"CN=Site";
const DWORD x_SiteCategoryLength = (sizeof( x_SiteCategoryName)/sizeof(WCHAR)) - 1;

const WCHAR MQ_S_NAME_ATTRIBUTE[] = L"cn";                         //PROPID_S_PATHNAME
const WCHAR MQ_S_ID_ATTRIBUTE[] = L"objectGUID";                   //PROPID_S_SITEID
const WCHAR MQ_S_SECURITY_ATRRIBUTE[] = L"nTSecurityDescriptor";   //PROPID_S_SECURITY
const WCHAR MQ_S_FULL_NAME_ATTRIBUTE[] = L"distinguishedName";     //PROPID_S_FULL_NAME
const WCHAR MQ_S_NT4_STUB_ATTRIBUTE[] = L"mSMQNt4Stub";            //PROPID_S_NT4_STUB
const WCHAR MQ_S_FOREIGN_ATTRIBUTE[] = L"mSMQSiteForeign";         //PROPID_S_FOREIGN
const WCHAR MQ_S_INTERVAL1[] = L"mSMQInterval1";                   //PROPID_S_INTERVAL1
const WCHAR MQ_S_INTERVAL2[] = L"mSMQInterval2";                   //PROPID_S_INTERVAL2

const ADSTYPE MQ_S_NAME_ADSTYPE  =  ADSTYPE_CASE_IGNORE_STRING;
const ADSTYPE MQ_S_ID_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_S_SECURITY_ADSTYPE = ADSTYPE_NT_SECURITY_DESCRIPTOR;
const ADSTYPE MQ_S_FULL_NAME_ADSTYPE = ADSTYPE_DN_STRING;
const ADSTYPE MQ_S_NT4_STUB_ADSTYPE = ADSTYPE_INTEGER;
const ADSTYPE MQ_S_FOREIGN_ADSTYPE = ADSTYPE_BOOLEAN;
const ADSTYPE MQ_S_INTERVAL1_ADSTYPE = ADSTYPE_INTEGER;
const ADSTYPE MQ_S_INTERVAL2_ADSTYPE = ADSTYPE_INTEGER;

//-----------------------------------------------------
//      Server
//-----------------------------------------------------
const WCHAR MSMQ_SERVER_CLASS_NAME[] = L"server";

const WCHAR x_ServerCategoryName[] = L"CN=Server";
const DWORD x_ServerCategoryLength = (sizeof( x_ServerCategoryName)/sizeof(WCHAR)) - 1;

const WCHAR MQ_SRV_NAME_ATTRIBUTE[] = L"cn";                      //PROPID_SRV_NAME
const WCHAR MQ_SRV_ID_ATTRIBUTE[] = L"objectGUID";              //PROPID_SRV_ID
const WCHAR MQ_SRV_FULL_PATH_ATTRIBUTE[] = L"distinguishedName";            //PROPID_SRV_FULL_PATH

const ADSTYPE MQ_SRV_NAME_ADSTYPE = ADSTYPE_DN_STRING;
const ADSTYPE MQ_SRV_ID_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_SRV_FULL_PATH_ADSTYPE = ADSTYPE_DN_STRING;

//-----------------------------------------------------
//      MSMQ setting
//-----------------------------------------------------
const WCHAR MSMQ_SETTING_CLASS_NAME[] = L"mSMQSettings";

const WCHAR x_SettingsCategoryName[] = L"CN=MSMQ-Settings";
const DWORD x_SettingsCategoryLength = (sizeof( x_SettingsCategoryName)/sizeof(WCHAR)) - 1;


const WCHAR MQ_SET_NAME_ATTRIBUTE[] = L"cn";                  //PROPID_SET_NAME
const WCHAR MQ_SET_SERVICE_ATTRIBUTE[] = L"mSMQServices";     //PROPID_SET_SERVICE  //[adsrv] keeping for migration: PROPID_SET_OLDSERVICE
const WCHAR MQ_SET_SERVICE_ROUTING_ATTRIBUTE[]   = L"mSMQRoutingService";         // PROPID_SET_SERVICE_ROUTING
const WCHAR MQ_SET_SERVICE_DSSERVER_ATTRIBUTE[]  = L"mSMQDsService";              // PROPID_SET_SERVICE_DSSERVER
const WCHAR MQ_SET_SERVICE_DEPCLIENTS_ATTRIBUTE[] = L"mSMQDependentClientService"; // PROPID_SET_SERVICE_DEPCLIENTS

const WCHAR MQ_SET_QM_ID_ATTRIBUTE[] = L"mSMQQMID";           //PROPID_SET_QM_ID
const WCHAR MQ_SET_APPLICATION_ATTRIBUTE[] = L"applicationName"; //PROPID_SET_APPLICATION
const WCHAR MQ_SET_FULL_PATH_ATTRIBUTE[] = L"distinguishedName"; // PROPID_SET_FULL_PATH
const WCHAR MQ_SET_NT4_ATTRIBUTE[] = L"mSMQNT4Flags";            //PROPID_SET_NT4
const WCHAR MQ_SET_MASTERID_ATTRIBUTE[] = L"mSMQOwnerID"; //PROPID_SET_MASTERID
const WCHAR MQ_SET_SITENAME_ATTRIBUTE[] = L"mSMQSiteNameEx"; //PROPID_SET_SITENAME


const ADSTYPE MQ_SET_NAME_ADSTYPE = ADSTYPE_DN_STRING;
const ADSTYPE MQ_SET_SERVICE_ADSTYPE = ADSTYPE_INTEGER;  //[adsrv]  keeping for migration only
const ADSTYPE MQ_SET_SERVICE_ROUTING_ADSTYPE = ADSTYPE_BOOLEAN;
const ADSTYPE MQ_SET_SERVICE_DSSERVER_ADSTYPE = ADSTYPE_BOOLEAN;
const ADSTYPE MQ_SET_SERVICE_DEPCLIENTS_ADSTYPE = ADSTYPE_BOOLEAN;
const ADSTYPE MQ_SET_QM_ID_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_SET_APPLICATION_ADSTYPE = ADSTYPE_CASE_IGNORE_STRING;
const ADSTYPE MQ_SET_FULL_PATH_ADSTYPE = ADSTYPE_DN_STRING;
const ADSTYPE MQ_SET_NT4_ADSTYPE = ADSTYPE_INTEGER;
const ADSTYPE MQ_SET_MASTERID_ADSTYPE = ADSTYPE_OCTET_STRING ;
const ADSTYPE MQ_SET_SITENAME_ADSTYPE = ADSTYPE_CASE_IGNORE_STRING ;

//-----------------------------------------------------
//      Computer
//-----------------------------------------------------

const WCHAR MSMQ_COMPUTER_CLASS_NAME[] = L"computer";

const WCHAR x_ComputerCategoryName[] = L"CN=Computer";
const DWORD x_ComputerCategoryLength = (sizeof( x_ComputerCategoryName)/sizeof(WCHAR)) - 1;


const WCHAR MQ_COM_FULL_PATH_ATTRIBUTE[] = L"distinguishedName";        // PROPID_COM_FULL_PATH
const WCHAR MQ_COM_SAM_ACCOUNT_ATTRIBUTE[] = L"sAMAccountName";         // PROPID_COM_SAM_ACCOUNT
const WCHAR MQ_COM_ACCOUNT_CONTROL_ATTRIBUTE[] = L"userAccountControl"; // PROPID_COM_ACCOUNT_CONTROL
const WCHAR MQ_COM_DNS_HOSTNAME_ATTRIBUTE[] = L"dNSHostName";           // PROPID_COM_DNS_HOSTNAME
const WCHAR MQ_COM_SID_ATTRIBUTE[] = L"objectSid";                      // PROPID_COM_SID
const WCHAR MQ_COM_SIGN_CERT_ATTRIBUTE[] = L"mSMQSignCertificates";     // PROPID_COM_SIGN_CERT
const WCHAR MQ_COM_DIGEST_ATTRIBUTE[] = L"mSMQDigests";                 // PROPID_COM_DIGEST
const WCHAR MQ_COM_ID_ATTRIBUTE[] = L"objectGUID";                      // PROPID_COM_ID
const WCHAR MQ_COM_VERSION_ATTRIBUTE[] = L"operatingSystemVersion";     // PROPID_COM_VERSION

const WCHAR MQ_COM_FULL_PATH_DESC[] = L"distingushed-name";
const WCHAR MQ_COM_SAM_ACCOUNT_DESC[] = L"sam-account-name";
const WCHAR MQ_COM_ACCOUNT_CONTROL_DESC[] = L"user-account-contorl-name";

const ADSTYPE MQ_COM_FULL_PATH_ADSTYPE = ADSTYPE_DN_STRING;
const ADSTYPE MQ_COM_SAM_ACCOUNT_ADSTYPE = ADSTYPE_DN_STRING;
const ADSTYPE MQ_COM_ACCOUNT_CONTROL_ADSTYPE = ADSTYPE_INTEGER;
const ADSTYPE MQ_COM_DNS_HOSTNAME_ADSTYPE = ADSTYPE_DN_STRING;
const ADSTYPE MQ_COM_SID_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_COM_SIGN_CERT_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_COM_DIGEST_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_COM_ID_ADSTYPE = ADSTYPE_OCTET_STRING;
const ADSTYPE MQ_COM_VERSION_ADSTYPE = ADSTYPE_DN_STRING;


//-----------------------------------------------------
//      DL
//-----------------------------------------------------
const WCHAR MSMQ_DL_CLASS_NAME[] = L"group";
const WCHAR MQ_DL_ID_ATTRIBUTE[] = L"objectGUID";
const ADSTYPE MQ_DL_ID_ADSTYPE = ADSTYPE_OCTET_STRING;


//-----------------------------------------------------
//     Alias queue
//-----------------------------------------------------
const WCHAR MSMQ_QALIAS_CLASS_NAME[] = L"msMQ-Custom-Recipient";
const WCHAR MQ_QALIAS_FORMAT_NAME_ATTRIBUTE[] = L"msMQ-Recipient-FormatName";
const ADSTYPE MQ_QALIAS_FORMAT_NAME_ADSTYPE = ADSTYPE_CASE_IGNORE_STRING;


#endif  //  __mqattrib_h__


