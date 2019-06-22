# ifndef _AVOIDOBSTACLE_H_
# define _AVOIDOBSTACLE_H_

# define Ultrasound_Uart_Port LPUART6

typedef enum
{
    Meassuring,
    Meassured,
    Error
}Enum_UltrasoundMeassure;

extern Enum_UltrasoundMeassure UltrasoundMeassure;

# define MaxMeassureRangeSetCMD_50cm 0x05 // 3885us
# define MaxMeassureRangeSetCMD_100cm 0x0A // 6770us
# define MaxMeassureRangeSetCMD_150cm 0x0F // 9655us
# define MaxMeassureRangeSetCMD_200cm 0x14 // 12540us

void ReceiveDistance();
void SendCMDToUltrasound();
void UltrasoundInit();

extern float Distance_Meassured;//µ¥Î»cm
# endif