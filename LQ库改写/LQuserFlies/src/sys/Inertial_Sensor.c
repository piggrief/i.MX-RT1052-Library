#include "Inertial_Sensor.h"
#include "status.h"
#include "AP_Math.h"
#define G					  9.80665f		      // m/s^2	
#define RadtoDeg    57.324841f				//弧度到角度 (弧度 * 180/3.1415)
#define DegtoRad    0.0174533f				//角度到弧度 (角度 * 3.1415/180)
#define Acc_Gain  	0.0001220f				//加速度变成G (初始化加速度满量程-+4g LSBa = 2*4/65535.0)
#define Gyro_Gain 	0.0609756f				//角速度变成度 (初始化陀螺仪满量程+-2000 LSBg = 2*2000/65535.0)
#define Gyro_Gr	    0.0010641f			  //角速度变成弧度(3.1415/180 * LSBg)   
#define VAR         0.001f                  //方差

/*低通滤波*/
float alpha;
/**设置低通滤波的频率sample_freq  和  截止频率cutoff_freq*/
void compute_alpha(float sample_freq, float cutoff_freq) 
{
    if (cutoff_freq <= 0.0f || sample_freq <= 0.0f) {
        alpha = 1.0;
    } else {
        float dt = 1.0/sample_freq;
        float rc = 1.0f/(M_2PI*cutoff_freq);
        alpha = constrain_float(dt/(dt+rc), 0.0f, 1.0f);
    }
}
/*需要滤波的信号sample  上次的输出信号_output*/
float apply(float sample, float  _output) 
{
    _output += (sample - _output) * alpha;
    return _output;
}



/*惯性传感器初始化*/
uint8_t ins_init(void)
{
    compute_alpha(1000, 20);    //输入信号 1khz  截止频率 20hz
    return MPU6050_Init();
}


/*惯性传感器校准*/
vector3i_t _acc_vector_offset;   //加计零偏值
vector3i_t _gyro_vector_offset;  //角速度计零偏值
vector3i_t _mag_vector_offset;  //地磁计零偏值
void ins_calibration(void)
{
    uint16_t i = 0;

    vector3i_t _acc_vector;   //存放加计读取的原始数据
    vector3i_t _gyro_vector;  //存放角速度计读取的原始数据
    vector3f_t _gyro_sum;     //存放角速度计和
    vector3f_t _acc_sum;      //存放加计和
    vector3f_t _gyro_var;     //存放角速度计平方
    vector3f_t _acc_var;      //存放加计平方
    while(i++ < 500)
    {
        MPU_Get_Accelerometer(&_acc_vector.x, &_acc_vector.y, &_acc_vector.z);  //得到原始数据
        MPU_Get_Gyroscope(&_gyro_vector.x, &_gyro_vector.y, &_gyro_vector.z);   //得到原始数据
        
        _gyro_sum.x += _gyro_vector.x/500.0f;
        _gyro_sum.y += _gyro_vector.y/500.0f;
        _gyro_sum.z += _gyro_vector.z/500.0f;
        
        _acc_sum.x  += _acc_vector.x/500.0f;
        _acc_sum.y  += _acc_vector.y/500.0f;
        _acc_sum.z  += (_acc_vector.z - 8192)/500.0f; 
        
        _gyro_var.x += _gyro_vector.x * _gyro_vector.x/500.0f ;
        _gyro_var.y += _gyro_vector.y * _gyro_vector.y/500.0f;
        _gyro_var.z += _gyro_vector.z * _gyro_vector.z/500.0f;
        
        _acc_var.x  += _acc_vector.x * _acc_vector.x/500.0f;
        _acc_var.y  += _acc_vector.y * _acc_vector.y/500.0f;
        _acc_var.z  += (_acc_vector.z - 8192 ) * (_acc_vector.z - 8192)/500.0f; 
        delayms(2);
        if(i % 50 == 0)
        {
            LED_Ctrl(LEDALL, RVS);         //白灯反转  感兴趣的可以使用红灯，蓝灯，紫灯等
        }
    }
    if(   invSqrt(_gyro_var.x - _gyro_sum.x * _gyro_sum.x) > VAR 
       && invSqrt(_gyro_var.y - _gyro_sum.y * _gyro_sum.y) > VAR 
       && invSqrt(_acc_var.x - _acc_sum.x * _acc_sum.x) > VAR 
       && invSqrt(_acc_var.y - _acc_sum.y * _gyro_sum.y) > VAR )     //所有数据方差在一定值内，说明校准时，没有运动
    {
        _acc_vector_offset.x   = (int16_t)_acc_sum.x;          //保存静止时的零偏值
        _acc_vector_offset.y   = (int16_t)_acc_sum.y;
        _acc_vector_offset.z   = (int16_t)_acc_sum.z;
        _gyro_vector_offset.x  = (int16_t)_gyro_sum.x;
        _gyro_vector_offset.y  = (int16_t)_gyro_sum.y;
        _gyro_vector_offset.z  = (int16_t)_gyro_sum.z;
        
        _status.ins_calibration = 0;                    //校准标志位清零
    }
}


vector3f_t _gyro_vector;     //全局变量，存放处理后的角速度数据
vector3f_t _acc_vector;      //全局变量，存放处理后的加数据
vector3f_t _mag_vector;      //全局变量，存放处理后的地磁数据
void ins_update(void)
{
    vector3i_t acc_vector;
    vector3i_t gyro_vector;
    
    vector3f_t new_gyro_vector;
    vector3f_t new_acc_vector;
    
    if(_status.ins_calibration)  //如果需要校准
    {
        ins_calibration();      //校准
    }
    
    MPU_Get_Accelerometer(&acc_vector.x, &acc_vector.y, &acc_vector.z);  //得到原始数据
    MPU_Get_Gyroscope(&gyro_vector.x, &gyro_vector.y, &gyro_vector.z);   //得到原始数据
    
    acc_vector.x -= _acc_vector_offset.x;   //去零偏
    acc_vector.y -= _acc_vector_offset.y;   //去零偏
    acc_vector.z -= _acc_vector_offset.z;   //去零偏
    gyro_vector.x -= _gyro_vector_offset.x;   //去零偏
    gyro_vector.y -= _gyro_vector_offset.y;   //去零偏
    gyro_vector.z -= _gyro_vector_offset.z;   //去零偏
    
    //加速度AD值 转换成 米/平方秒 
	new_acc_vector.x = (float)acc_vector.x * Acc_Gain * G;
	new_acc_vector.y = (float)acc_vector.y * Acc_Gain * G;
	new_acc_vector.z = (float)acc_vector.z * Acc_Gain * G;
    
	//陀螺仪AD值 转换成 弧度/秒    
	new_gyro_vector.x = (float) gyro_vector.x * Gyro_Gr;  
	new_gyro_vector.y = (float) gyro_vector.y * Gyro_Gr;
	new_gyro_vector.z = (float) gyro_vector.z * Gyro_Gr;
    
    //低通滤波
//    _acc_vector.x = apply(new_acc_vector.x, _acc_vector.x);
//    _acc_vector.y = apply(new_acc_vector.y, _acc_vector.y);
//    _acc_vector.z = apply(new_acc_vector.z, _acc_vector.z);
//    _gyro_vector.x = apply(new_gyro_vector.x, _gyro_vector.x);
//    _gyro_vector.y = apply(new_gyro_vector.y, _gyro_vector.y);
//    _gyro_vector.z = apply(new_gyro_vector.z, _gyro_vector.z);

     _acc_vector.x = new_acc_vector.x;
     _acc_vector.y = new_acc_vector.y;
     _acc_vector.z = new_acc_vector.z;
     _gyro_vector.x = new_gyro_vector.x;
     _gyro_vector.y = new_gyro_vector.y;
     _gyro_vector.z = new_gyro_vector.z;
  
}
void ins_update9(void)
{
//    vector3i_t acc_vector;
//    vector3i_t gyro_vector;
    
    vector3f_t new_gyro_vector;
    vector3f_t new_acc_vector;
    
    if(_status.ins_calibration)  //如果需要校准
    {
        ins_calibration();      //校准
    }
    int16_t u16data[9];  //存放读取传感器 数据
    Sensor_ReadData(u16data);  //得到原始数据
    
//    acc_vector.x = _acc_vector_offset.x;   //去零偏
//    acc_vector.y = _acc_vector_offset.y;   //去零偏
//    acc_vector.z = _acc_vector_offset.z;   //去零偏
//    gyro_vector.x = _gyro_vector_offset.x;   //去零偏
//    gyro_vector.y = _gyro_vector_offset.y;   //去零偏
//    gyro_vector.z = _gyro_vector_offset.z;   //去零偏
    
    //加速度AD值 转换成 米/平方秒 
	new_acc_vector.x = (float)u16data[0] * Acc_Gain * G * 4;
	new_acc_vector.y = (float)u16data[1] * Acc_Gain * G * 4;
	new_acc_vector.z = (float)u16data[2] * Acc_Gain * G * 4;
    
	//陀螺仪AD值 转换成 弧度/秒    
	new_gyro_vector.x = (float) u16data[6] * Gyro_Gr;  
	new_gyro_vector.y = (float) u16data[7] * Gyro_Gr;
	new_gyro_vector.z = (float) u16data[8] * Gyro_Gr;
    
    //低通滤波
//    _acc_vector.x = apply(new_acc_vector.x, _acc_vector.x);
//    _acc_vector.y = apply(new_acc_vector.y, _acc_vector.y);
//    _acc_vector.z = apply(new_acc_vector.z, _acc_vector.z);
//    _gyro_vector.x = apply(new_gyro_vector.x, _gyro_vector.x);
//    _gyro_vector.y = apply(new_gyro_vector.y, _gyro_vector.y);
//    _gyro_vector.z = apply(new_gyro_vector.z, _gyro_vector.z);

     _acc_vector.x = new_acc_vector.x;
     _acc_vector.y = new_acc_vector.y;
     _acc_vector.z = new_acc_vector.z;
     _gyro_vector.x = new_gyro_vector.x;
     _gyro_vector.y = new_gyro_vector.y;
     _gyro_vector.z = new_gyro_vector.z;
     _mag_vector.x = u16data[3];
     _mag_vector.y = u16data[4];
     _mag_vector.z = u16data[7];
         
  
}

vector3f_t get_gyro()
{    
	return _gyro_vector;
}

vector3f_t get_acc()
{
	return _acc_vector;
}


ins_t _ins =
{

	ins_init,
	ins_calibration,
	ins_update,
    ins_update9,
	get_gyro,
	get_acc,

};
