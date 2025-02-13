/******************************Module*Header*******************************\
* Module Name: genlem.c
*
* The Twist style of the 3D Flying Objects screen saver.
*
* Solid model of a 3D lemniscate.
*
* Copyright (c) 1994 Microsoft Corporation
*
\**************************************************************************/

#include <stdlib.h>
#include <windows.h>
//#include <GL\gl.h>
#include <string.h>
#include <math.h>
//#include "ss3dfo.h"
#include <d3dx8.h>
#include "D3DSaver.h"
#include "FlyingObjects.h"
#include "mesh.h"

#define ROT_PREC    10
#define NORMS(x, y) lemMesh.norms[((x) * iPrec) + y]
#define GRID(x, y)  lemMesh.pts[((x) * iPrec) + y]

static MESH lemMesh;
static POINT3D basis[ROT_PREC];
static double zrot = 0.2;
static int iPrec = 32;
static double *lemX;
static double *lemY;
static double *lemXT;
static double *lemYT;


static void getLem(double index, double max, double *angle, double *r)
{    
    double a, sina;

    a = (index * PI) / (max - 1.0);
    if (a >= PI)
        a -= PI;
    if (a > PI / 2.0) {
        *angle = (2.0 * PI) - a;
        sina = sin( 2.0 * *angle );
        if( sina < 0.0 )
            sina = 0.0; // protect against sqrt fpe
        *r = 0.5 * sqrt(sina);
    } else {
        *angle = a;
        sina = sin( 2.0 * *angle );
        if( sina < 0.0 )
            sina = 0.0;
        *r = 0.5 * sqrt(sina);
    }
}            


static void initLemCoords(int iMax)
{
    int i;
    double max = (double)iMax;
    double angle;
    double r;

    for (i = 0; i < iMax; i++) {
        getLem((double)i, (double)iPrec, &angle, &r);        
        lemX[i] = r * cos(angle);
        lemY[i] = r * sin(angle);
        
        getLem((double)i + 0.00001, (double)iPrec, &angle, &r);        
        lemXT[i] = r * cos(angle);
        lemYT[i] = r * sin(angle);

    }
}


void genLemniscate(void)
{
    int i;
    int j;
    double posInc = 2.0 / (float)iPrec;
    int facecount = 0;
    int ptcount = 0;
    POINT3D norm;
    static float twistFact = 0.0f;
    static float twistFactAdd = 0.05f;
    POINT3D a[ROT_PREC];
    POINT3D b[ROT_PREC];
    MATRIX matrix;
    MESH *mesh = &lemMesh;

    mesh->numPoints = 0;   
    mesh->numFaces = 0;   
    for (i = 0; i < (iPrec - 1) * (ROT_PREC - 1); i++)
        mesh->norms[i] = ss_ptZero;

    for (i = 0; i < (iPrec - 1); i++) {
        double x1, y1, x2, y2;
        double len;
        double sinAngle;
        double rotZ;
        int id[4];

        x1 = lemX[i];
        y1 = lemY[i];
        x2 = lemXT[i];
        y2 = lemYT[i];

        x2 -= x1;
        y2 -= y1;

        len = sqrt(x2 * x2 + y2 * y2);
        if (len > 0.0)
            sinAngle = y2 / len;
        else
            sinAngle = 0.0;
        if (y2 < 0.0)
            sinAngle = -sinAngle;
        rotZ = asin(sinAngle);
        if (x2 < 0.0)
            rotZ = PI - rotZ;
        if (y2 < 0.0)
            rotZ = -rotZ;
        if (rotZ < 0.0)
            rotZ = 2.0 * PI + rotZ;

        ss_matrixIdent(&matrix);
        ss_matrixRotate(&matrix, 0.0, 0.0, -rotZ);
        ss_matrixTranslate(&matrix, x1, y1, 
                         twistFact * cos((2.0 * PI * (float)i) / ((float)iPrec - 1)));
        
        for (j = 0; j < ROT_PREC; j++)
            ss_xformPoint(&a[j], &basis[j], &matrix);

        x1 = lemX[i+1];
        y1 = lemY[i+1];

        x2 = lemXT[i+1];
        y2 = lemYT[i+1];

        x2 -= x1;
        y2 -= y1;

        len = sqrt(x2 * x2 + y2 * y2);

        if (len > 0.0)
            sinAngle = y2 / len;
        else
            sinAngle = 0.0;
        if (y2 < 0.0)
            sinAngle = -sinAngle;
        rotZ = asin(sinAngle);
        if (x2 < 0.0)
            rotZ = PI - rotZ;
        if (y2 < 0.0)
            rotZ = -rotZ;
        if (rotZ < 0.0)
            rotZ = 2.0 * PI + rotZ;

        ss_matrixIdent(&matrix);
        ss_matrixRotate(&matrix, 0.0, 0.0, -rotZ);        
        ss_matrixTranslate(&matrix, x1, y1, 
                         twistFact * cos((2.0 * PI * ((float)i + 1.0)) / ((float)iPrec - 1)));

        for (j = 0; j < ROT_PREC; j++)
            ss_xformPoint(&b[j], &basis[j], &matrix);
            
        memcpy(&mesh->pts[ptcount], &a, sizeof(POINT3D) * (ROT_PREC - 1));
        ptcount += (ROT_PREC - 1);
        mesh->numPoints += (ROT_PREC - 1);
        
            
        for (j = 0; j < (ROT_PREC - 1); j++) {
            int k;
            int jj;
            
            if (j == (ROT_PREC - 2))
                jj = 0;
            else
                jj = j + 1;

            ss_calcNorm(&norm, &b[j + 1], &b[j], &a[j]);
            
            mesh->faces[facecount].material = 3;
            mesh->faces[facecount].norm = norm;
            if (i == iPrec - 2) {
                id[0] = mesh->faces[facecount].p[0] = j;
                id[1] = mesh->faces[facecount].p[1] = jj;
            } else {
                id[0] = mesh->faces[facecount].p[0] = ptcount + j;
                id[1] = mesh->faces[facecount].p[1] = ptcount + jj;
            }
            id[2] = mesh->faces[facecount].p[2] = ptcount - (ROT_PREC - 1) + j;
            id[3] = mesh->faces[facecount].p[3] = ptcount - (ROT_PREC - 1) + jj;
            
            for (k = 0; k < 4; k++) {
                POINT3D *pn = &mesh->norms[id[k]];
                
                pn->x += norm.x;
                pn->y += norm.y;
                pn->z += norm.z;
            }
            mesh->numFaces++;
            facecount++;
        }
    }

    ss_normalizeNorms(lemMesh.norms, lemMesh.numPoints);
        
    if (twistFact >= 1.0f)
        twistFactAdd = -0.01f;
    else if (twistFact <= -1.0f)
        twistFactAdd = 0.01f;
    twistFact += twistFactAdd;
        
}

BOOL initLemScene()
{
    int i;
    RGBA lightAmbient = {0.0f, 0.0f, 0.0f, 1.0f};

    iPrec = (int)(fTesselFact * 32.5);
    if (iPrec < 5)
        iPrec = 5;

    lemX = (double*)SaverAlloc(sizeof(double) * iPrec);
    if( lemX == NULL )
        return FALSE;

    lemY = (double*)SaverAlloc(sizeof(double) * iPrec);
    if( lemY == NULL )
        return FALSE;

    lemXT = (double*)SaverAlloc(sizeof(double) * iPrec);
    if( lemXT == NULL )
        return FALSE;

    lemYT = (double*)SaverAlloc(sizeof(double) * iPrec);
    if( lemYT == NULL )
        return FALSE;

/*
    D3DXMATRIX matProj;
    D3DXMatrixOrthoLH( &matProj, 3.0, 3.0, 0.0f, 3.0f );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
*/
    SetProjectionMatrixInfo( TRUE, 3.0f, 3.0f, 0.0f, 3.0f );

    D3DXMATRIX matView;
    D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
    D3DXVECTOR3 vEyePt(0, 0, 1.5f);
    D3DXVECTOR3 vLookatPt(0, 0, 0);
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
    m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

    if (!newMesh(&lemMesh, (ROT_PREC - 1) * (iPrec - 1) , 
            (ROT_PREC - 1) * (iPrec - 1)) )
    {
        return FALSE;
    }

    for (i = 0; i < ROT_PREC; i++) {
        basis[i].x = 0.0f;
        basis[i].y = (float) (0.15 * cos((i * 2.0 * PI) / (ROT_PREC - 1.0)));
        basis[i].z = (float) (0.15 * sin((i * 2.0 * PI) / (ROT_PREC - 1.0)));
    }

    initLemCoords(iPrec);
/*
    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (FLOAT *) &lightAmbient);
*/
    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
    myglMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                 (FLOAT *) &Material[3].Diffuse);

    return TRUE;
}

void delLemScene()
{
    delMesh(&lemMesh);

    SaverFree(lemX);
    SaverFree(lemY);
    SaverFree(lemXT);
    SaverFree(lemYT);
}

void updateLemScene(int flags, FLOAT fElapsedTime)
{
    static double mxrot = 0.0;
    static double myrot = 0.0;
    static double mzrot = 0.0;
    static double mxrotInc = 0.0;
    static double myrotInc = 0.1;
    static double zrotInc = 0.1;
    static double mzrotInc = 0.0;
    RGBA color;
    MATRIX model;
    D3DXMATRIX mat1, mat2, mat3, mat4, mat5, matFinal;
    static FLOAT fH = 0.0f;
    FLOAT fTimeFactor = fElapsedTime * 20.0f;
    if( fTimeFactor > 0.25f )
        fTimeFactor = 0.25f;
    
    mxrot += mxrotInc * fTimeFactor;
    myrot += myrotInc * fTimeFactor;
    mzrot += mzrotInc * fTimeFactor;

    if( gbBounce ) {
        // floating window bounced off an edge
        if (mxrotInc) {
            mxrotInc = 0.0;
            myrotInc = 0.1;
        } else if (myrotInc) {
            myrotInc = 0.0;
            mzrotInc = 0.1;
        } else if (mzrotInc) {
            mzrotInc = 0.0;
            mxrotInc = 0.1;
        }
        gbBounce = FALSE;
    }

    zrot += zrotInc * fTimeFactor;
    if (zrot >= PI / 4.0) {
        zrot = PI / 4.0;
        zrotInc = -0.03;
    } else if (zrot <= -PI / 4.0) {
        zrot = -PI / 4.0;
        zrotInc = 0.03;
    }

    genLemniscate();

    if (bColorCycle) {
        ss_HsvToRgb(fH, 1.0f, 1.0f, &color );

        myglMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (FLOAT *) &color);

        fH += fTimeFactor;
        if( fH >= 360.0f )
            fH -= 360.0f;
    }
/*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.5, 1.5, -1.5, 1.5, 0.0, 3.0);
    glTranslatef(0.0f, 0.0f, -1.5f);
*/
/*
    glRotatef((FLOAT) (zrot * (180.0 / PI)), 0.0f, 1.0f, 0.0f);
    glRotatef(50.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(50.0f, 0.0f, 0.0f, 1.0f);
*/

    D3DXMATRIX matView;
    D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
    D3DXVECTOR3 vEyePt(0, 0, 1.5f);
    D3DXVECTOR3 vLookatPt(0, 0, 0);
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
    m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
/*
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, -0.5f, 0.0f);
    glRotatef((FLOAT) (mxrot * (180.0 / PI)), 1.0f, 0.0f, 0.0f);
    glRotatef((FLOAT) (myrot * (180.0 / PI)), 0.0f, 1.0f, 0.0f);
    glRotatef((FLOAT) (mzrot * (180.0 / PI)), 0.0f, 0.0f, 1.0f);
*/
    D3DXMatrixTranslation(&mat1, 0.0f, -0.5f, 0.0f);
    D3DXMatrixRotationX(&mat2, D3DXToRadian((FLOAT)(mxrot * (180.0 / PI))));
    D3DXMatrixRotationY(&mat3, D3DXToRadian((FLOAT)(myrot * (180.0 / PI))));
    D3DXMatrixRotationZ(&mat4, D3DXToRadian((FLOAT)(mzrot * (180.0 / PI))));
    matFinal = mat4 * mat3 * mat2 * mat1 ;
    m_pd3dDevice->SetTransform( D3DTS_WORLD , &matFinal );

    ss_matrixIdent(&model);
    ss_matrixRotate(&model, mxrot, myrot, mzrot);
    ss_matrixTranslate(&model, 0.0, -0.5, 0.0);

//    updateObject(&lemMesh, bSmoothShading);    
    RenderMesh3(&lemMesh, bSmoothShading);
}
