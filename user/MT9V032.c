/*!
* @file       MT9V032.c
* @brief      根据官方的相关摄像头库编写的针对总钻风摄像头的驱动程序
* @details    
* @author     pig's grief
* @version    v1.0
* @date       2019-4-22
* @to do      
*/
# include "include.h"

AT_NONCACHEABLE_SECTION_ALIGN(uint16_t csiFrameBuf[APP_CAMERA_FRAME_BUFFER_COUNT][APP_CAMERA_HEIGHT][APP_CAMERA_WIDTH], FRAME_BUFFER_ALIGN); //定义摄像头数据缓存区

typedef struct _MT9V032_resource
{
    sccb_i2c_t sccbI2C;                     //!< I2C for SCCB transfer. 
    void(*pullResetPin)(bool pullUp);     /*!< Function to pull reset pin high or low. */
    void(*pullPowerDownPin)(bool pullUp); /*!< Function to pull the power down pin high or low. */
    uint32_t inputClockFreq_Hz;             //!< Input clock frequency. 
} MT9V032_resource_t;

void MT9V032PinInit()
{
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_07_CSI_HSYNC,         /* GPIO_AD_B1_07 is configured as CSI_HSYNC */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_10_CSI_DATA07,        /* GPIO_AD_B1_10 is configured as CSI_DATA07 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_11_CSI_DATA06,        /* GPIO_AD_B1_11 is configured as CSI_DATA06 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_12_CSI_DATA05,        /* GPIO_AD_B1_12 is configured as CSI_DATA05 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_13_CSI_DATA04,        /* GPIO_AD_B1_13 is configured as CSI_DATA04 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_14_CSI_DATA03,        /* GPIO_AD_B1_14 is configured as CSI_DATA03 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_15_CSI_DATA02,        /* GPIO_AD_B1_15 is configured as CSI_DATA02 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_10_CSI_DATA00,           /* GPIO_B1_10 is configured as CSI_DATA00 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_11_CSI_DATA01,           /* GPIO_B1_11 is configured as CSI_DATA01 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_12_CSI_PIXCLK,           /* GPIO_B1_12 is configured as CSI_PIXCLK */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_13_CSI_VSYNC,            /* GPIO_B1_13 is configured as CSI_VSYNC */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
}

static csi_resource_t csiResource = {   //指向csi基地址
    .csiBase = CSI,
};




static csi_private_data_t csiPrivateData;  //csi私有数据

camera_receiver_handle_t cameraReceiver = {//摄像头接收配置结构体
    .resource = &csiResource,
    .ops = &csi_ops,
    .privateData = &csiPrivateData,
};


// 配置摄像机设备和接收器。
camera_config_t cameraConfig = {
    .pixelFormat = kVIDEO_PixelFormatYUYV,//kVIDEO_PixelFormatYUYV,//kVIDEO_PixelFormatBGR565,
    .bytesPerPixel = APP_BPP,//   每个像素点几个数据
    .resolution = FSL_VIDEO_RESOLUTION(APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT), //分辨率
    .frameBufferLinePitch_Bytes = APP_CAMERA_WIDTH * APP_BPP,                //行间隔
    .interface = kCAMERA_InterfaceGatedClock,                            //摄像机接口类型
    .controlFlags = APP_CAMERA_CONTROL_FLAGS,
    .framePerSec = 50,                                                     //fps 修改需要修改曝光时间 和 分辨率 配合
};

static MT9V032_resource_t MT9V032Resource = {   //摄像头初始化结构体
    .sccbI2C = LPI2C1,

    .inputClockFreq_Hz = 27000000,
};

status_t MT9V032_Init(camera_device_handle_t *handle, const camera_config_t *config)
{
    return kStatus_Success;
}

status_t MT9V032_Deinit(camera_device_handle_t *handle)
{
    ((MT9V032_resource_t *)(handle->resource))->pullPowerDownPin(true);
    return kStatus_Success;
}

status_t MT9V032_Control(camera_device_handle_t *handle, camera_device_cmd_t cmd, int32_t arg)
{
    return kStatus_InvalidArgument;
}

status_t MT9V032_Start(camera_device_handle_t *handle)
{
    return kStatus_Success;
}

status_t MT9V032_Stop(camera_device_handle_t *handle)
{
    return kStatus_Success;
}

status_t MT9V032_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig)
{
    return MT9V032_Init(handle, config);
}

const camera_device_operations_t MT9V032_ops = {
    .init = MT9V032_Init,
    .deinit = MT9V032_Deinit,
    .start = MT9V032_Start,
    .stop = MT9V032_Stop,
    .control = MT9V032_Control,
    .init_ext = MT9V032_InitExt,
};

camera_device_handle_t cameraDevice = {           //摄像头驱动配置结构体
    .resource = &MT9V032Resource,
    .ops = &MT9V032_ops,
};




void MT9V032Init()
{
    /*引脚初始化*/
    MT9V032PinInit();
    /*CSI模块初始化*/
    CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, NULL, NULL);
    CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);
    CAMERA_DEVICE_Start(&cameraDevice);

    for (uint32_t i = 0; i < APP_CAMERA_FRAME_BUFFER_COUNT; i++) //将空帧缓冲区提交到缓冲区队列
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(csiFrameBuf[i]));
    }

    CAMERA_RECEIVER_Start(&cameraReceiver);   // 启动接收camera数据

    delayms(1000);        //延时200毫秒  摄像头不是重新上电 可以不要延时
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) {//注意，使用csiFrameBuf数组时，最好关闭D-Cache 不然上次数据可能会存放在cache里面，造成数据错乱
        SCB_DisableDCache();
    }
}