#include "mcuthread.h"

#include <QtCore/QRandomGenerator>
#include <QThread>
#include <QtDebug>

#include "utils.h"

#define PI             ((float) 3.14159265f)

McuThread::McuThread(AnalogCompas * analogCompas){
    this->analogCompas = analogCompas;
}

static float MagBuffer[3] = {0.0f}, AccBuffer[3] = {0.0f}, Buffer[3] = {0.0f};
float compasAngle();

void McuThread::run()
{

    SystemInit();

    /* Demo Compass */
    Demo_CompassConfig();

    while(1){
        //QThread::msleep(1000);

        /* Read Compass data */
        Demo_CompassReadMag(MagBuffer);
        Demo_CompassReadAcc(AccBuffer);

        const float angle = compasAngle();

        qDebug() << "calculated angle: " << angle << "compas angle X:" << MagBuffer[0]
                 << "compas angle Y:" << MagBuffer[1]
                 << "compas angle Z:" << MagBuffer[2];
        analogCompas->setAngle(angle);
        analogCompas->update();
    }
}

void McuThread::tryConnect(remcu_thread_options_t options)
{
    qDebug() << "server params:" ;
}

float fNormAcc,fSinRoll,fCosRoll,fSinPitch,fCosPitch = 0.0f, RollAng = 0.0f, PitchAng = 0.0f;
float fTiltedX,fTiltedY = 0.0f;
#define PI                         (float)     3.14159265f


float compasAngle(){
    for(int i=0;i<3;i++)
        AccBuffer[i] /= 100.0f;

    fNormAcc = sqrt((AccBuffer[0]*AccBuffer[0])+(AccBuffer[1]*AccBuffer[1])+(AccBuffer[2]*AccBuffer[2]));

    fSinRoll = -AccBuffer[1]/fNormAcc;
    fCosRoll = sqrt(1.0-(fSinRoll * fSinRoll));
    fSinPitch = AccBuffer[0]/fNormAcc;
    fCosPitch = sqrt(1.0-(fSinPitch * fSinPitch));
    if ( fSinRoll >0)
    {
        if (fCosRoll>0)
        {
            RollAng = acos(fCosRoll)*180/PI;
        }
        else
        {
            RollAng = acos(fCosRoll)*180/PI + 180;
        }
    }
    else
    {
        if (fCosRoll>0)
        {
            RollAng = acos(fCosRoll)*180/PI + 360;
        }
        else
        {
            RollAng = acos(fCosRoll)*180/PI + 180;
        }
    }

    if ( fSinPitch >0)
    {
        if (fCosPitch>0)
        {
            PitchAng = acos(fCosPitch)*180/PI;
        }
        else
        {
            PitchAng = acos(fCosPitch)*180/PI + 180;
        }
    }
    else
    {
        if (fCosPitch>0)
        {
            PitchAng = acos(fCosPitch)*180/PI + 360;
        }
        else
        {
            PitchAng = acos(fCosPitch)*180/PI + 180;
        }
    }

    if (RollAng >=360)
    {
        RollAng = RollAng - 360;
    }

    if (PitchAng >=360)
    {
        PitchAng = PitchAng - 360;
    }

    fTiltedX = MagBuffer[0]*fCosPitch+MagBuffer[2]*fSinPitch;
    fTiltedY = MagBuffer[0]*fSinRoll*fSinPitch+MagBuffer[1]*fCosRoll-MagBuffer[1]*fSinRoll*fCosPitch;

    float HeadingValue = (float) ((atan2f((float)fTiltedY,(float)fTiltedX))*180)/PI;

    if (HeadingValue < 0)
    {
        //HeadingValue = HeadingValue + 360;
    }

    return HeadingValue;
}

extern "C" uint32_t L3GD20_TIMEOUT_UserCallback(void){
    qDebug() << "Error : L3GD20_TIMEOUT";
    return 1;
}

extern "C" uint32_t LSM303DLHC_TIMEOUT_UserCallback(void){
    qDebug() << "Error : LSM303DLHC_TIMEOUT";
    return 1;
}
