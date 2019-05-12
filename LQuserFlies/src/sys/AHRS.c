#include "AHRS.h"
#include "Inertial_Sensor.h"
#include "m_systime.h"
#include "AP_Math.h"
#include "math.h"
////////////////////////////////////////////////////////////////////////////////
// Orientation
////////////////////////////////////////////////////////////////////////////////
// Convienience accessors for commonly used trig functions. These values are generated
// by the DCM through a few simple equations. They are used throughout the code where cos and sin
// would normally be used.
// The cos values are defaulted to 1 to get a decent initial value for a level state
static Q4_t Q4 = {1, 0, 0, 0};  //四元数
const float ahrs_kp = 1.08f; //PI控制器，修正机体坐标系
const float ahrs_ki = 0.05f;
static vector3f_t integral;  //机体坐标误差积分
static vector3f_t ahrs_angle;  //机体坐标误差积分


/****函数  AHRS_quat_update
	*作用  更新四元数
	*参数
	*返回值
	***/
void AHRS_quat_update(vector3f_t gyro, vector3f_t acc, float interval)
{
	float q0 = Q4.q0;
	float q1 = Q4.q1;
	float q2 = Q4.q2;
	float q3 = Q4.q3;
/***********  模长  ************/	
	float norm = invSqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
/***********  加计测出的机体坐标系   **********/
	float ax = acc.x * norm;
	float ay = acc.y * norm;
	float az = acc.z * norm;
/***********  四元数解算出的机体坐标系  ***************/
	float half_vx = q1*q3 - q0*q2;
	float half_vy = q2*q3 + q0*q1;
	float half_vz = -0.5f + q0*q0 + q3*q3;
/***********  叉积求加计机体坐标和上次四元数解算机体坐标差  ************/	
	float half_ex = ay*half_vz - az*half_vy;
	float half_ey = az*half_vx - ax*half_vz;
	float half_ez = ax*half_vy - ay*half_vx;
/***********  使用PI控制器 修正机体坐标 *************/	
	integral.x += half_ex * ahrs_ki * interval;
	integral.y += half_ey * ahrs_ki * interval;
	integral.z += half_ez * ahrs_ki * interval;
	
	float gx = (gyro.x + ahrs_kp * half_ex + integral.x) * 0.5f * interval;
	float gy = (gyro.y + ahrs_kp * half_ey + integral.y) * 0.5f * interval;
	float gz = (gyro.z + ahrs_kp * half_ez + integral.z) * 0.5f * interval;
	
/**********  更新四元数  ********/
	Q4.q0 += (-q1 * gx - q2 * gy - q3 * gz); 
	Q4.q1 += ( q0 * gx + q2 * gz - q3 * gy); 
	Q4.q2 += ( q0 * gy - q1 * gz + q3 * gx); 
	Q4.q3 += ( q0 * gz + q1 * gy - q2 * gx); 
  //单位化四元数 	
	norm = invSqrt(Q4.q0 * Q4.q0 + Q4.q1 * Q4.q1 + Q4.q2 * Q4.q2 + Q4.q3 * Q4.q3);
	
	Q4.q0 *= norm;
	Q4.q1 *= norm;
	Q4.q2 *= norm;
	Q4.q3 *= norm;
}


/****函数  AHRS_quat_update
	*作用  9轴更新四元数
	*参数
	*返回值
	***/
void AHRS_quat_update9(vector3f_t gyro, vector3f_t acc, vector3f_t mag, float interval)
{
    float recipNorm;   //模长倒数
    float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;  //四元数
    float hx, hy, bx, bz;
    float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
    float halfex, halfey, halfez;
    float qa, qb, qc;
    
    if((mag.x == 0)&&(mag.y == 0)&&(mag.z == 0))  //如果地磁计全为0  忽略地磁计
        {
            
            return;
        }
    if((acc.x == 0)&&(acc.y == 0)&&(acc.z == 0))  //如果加速度计全为0  忽略加速度计
        {
            
            return;
        }  
    /*加速度计数据归一化处理*/
    recipNorm = invSqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
    /***********  加计测出的机体坐标系   **********/
	float ax = acc.x * recipNorm;
	float ay = acc.y * recipNorm;
	float az = acc.z * recipNorm;
    
    /*地磁计数据归一化处理*/
    recipNorm = invSqrt(mag.x * mag.x + mag.y * mag.y + mag.z * mag.z);
    /***********  地磁计的机体坐标系   **********/
	float mx = mag.x * recipNorm;
	float my = mag.y * recipNorm;
	float mz = mag.z * recipNorm;
    
//  预先进行四元数数据运算，以避免重复运算带来的效率问题。
// Auxiliary variables to avoid repeated arithmetic
    q0q0 =  Q4.q0 * Q4.q0;
    q0q1 =  Q4.q0 * Q4.q1;
    q0q2 =  Q4.q0 * Q4.q2;
    q0q3 =  Q4.q0 * Q4.q3;
    q1q1 =  Q4.q1 * Q4.q1;
    q1q2 =  Q4.q1 * Q4.q2;
    q1q3 =  Q4.q1 * Q4.q3;
    q2q2 =  Q4.q2 * Q4.q2;
    q2q3 =  Q4.q2 * Q4.q3;
    q3q3 =  Q4.q3 * Q4.q3; 
    // 地球磁场的参考方向
    hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
    hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
    bx = sqrt(hx * hx + hy * hy);
    bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));
    
  //  根据当前四元数的姿态值来估算出各重力分量Vx，Vy，Vz和各地磁分量Wx，Wy，Wz。
// Estimated direction of gravity and magnetic field
    halfvx = q1q3 - q0q2;
    halfvy = q0q1 + q2q3;
    halfvz = q0q0 - 0.5f + q3q3;
    halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
    halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
    halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);
    
    //使用叉积来计算重力和地磁误差。
    // Error is sum of cross product between estimated direction and measured direction of field vectors
    halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
    halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
    halfez = (ax * halfvy - ay * halfvx) + (mx * halfwy - my * halfwx);
    
    //把上述计算得到的重力和磁力差进行积分运算，
    // Compute and apply integral feedback if enabled
    float gx = 0;
    float gy = 0;
    float gz = 0;
    if(ahrs_ki > 0.0f) {
        integral.x += ahrs_ki * halfex * interval; // integral error scaled by Ki
        integral.y += ahrs_ki * halfey * interval;
        integral.z += ahrs_ki * halfez * interval;
        gx += integral.x; // apply integral feedback
        gy += integral.y;
        gz += integral.z;
    }
    else {
        integral.x = 0.0f; // prevent integral windup
        integral.y = 0.0f;
        integral.z = 0.0f;
    }
    
    //把上述计算得到的重力差和磁力差进行比例运算。
    // Apply proportional feedback
    gx += ahrs_kp * halfex;
    gy += ahrs_kp * halfey;
    gz += ahrs_kp * halfez;
    
    
    //把由加速计和磁力计修正过后的陀螺仪数据整合到四元数中。
    // Integrate rate of change of quaternion
    gx *= (0.5f * interval); // pre-multiply common factors
    gy *= (0.5f * interval);
    gz *= (0.5f * interval);
    qa = Q4.q0;
    qb = Q4.q1;
    qc = Q4.q2;
    Q4.q0 += (-qb * gx - qc * gy - Q4.q3 * gz);
    Q4.q1 += (qa * gx + qc * gz - Q4.q3 * gy);
    Q4.q2 += (qa * gy - qb * gz + Q4.q3 * gx);
    Q4.q3 += (qa * gz + qb * gy - qc * gx);
    
    //把上述运算后的四元数进行归一化处理。得到了物体经过旋转后的新的四元数。
    // Normalise quaternion
    recipNorm = invSqrt(Q4.q0 * Q4.q0 + Q4.q1 * Q4.q1 + Q4.q2 * Q4.q2 + Q4.q3 * Q4.q3);
    Q4.q0 *= recipNorm;
    Q4.q1 *= recipNorm;
    Q4.q2 *= recipNorm;
    Q4.q3 *= recipNorm;
}

/****函数  AHRS_quat_to_angle
	*作用  更新姿态角
	*参数
	*返回值
	***/
void AHRS_quat_to_angle(void)
{
	float conv_x = 2.0f * (Q4.q0 * Q4.q2 - Q4.q1 * Q4.q3);  
	float conv_y = 2.0f * (Q4.q0 * Q4.q1 + Q4.q2 * Q4.q3);
	float conv_z = Q4.q0 * Q4.q0 - Q4.q1 * Q4.q1 - Q4.q2 * Q4.q2 + Q4.q3 * Q4.q3;
/*******  姿态解算  ********/
	ahrs_angle.x = fast_atan(conv_y * invSqrt(conv_x * conv_x + conv_z * conv_z)) * 57.2958f;
	ahrs_angle.y = asin(2 * (Q4.q0 * Q4.q2 - Q4.q3 * Q4.q1)) * 57.2958f;
	ahrs_angle.z = atan2(2 * (Q4.q0 * Q4.q3 + Q4.q1 * Q4.q2), 1 - 2 * (Q4.q2 * Q4.q2 + Q4.q3 * Q4.q3)) * 57.2958f;   
    
/*******  角度微调  ********/
//	ahrs_angle.x -= 
//	ahrs_angle.y -= 
    static float offset = 0;   
    offset -= 0.0005585 * 4.0f;  //补偿yaw 根据自己解算速度 自行补偿  1ms解算一次 补偿 0.0005585
	ahrs_angle.z -= offset;      //补偿yaw 根据自己解算速度 自行补偿
	
}


/*姿态更新*/
void ahrs_update()
{
	// 读取惯性传感器数据
	_ins.update();
	
	// if the update call took more than 0.2 seconds then discard it,
    // otherwise we may move too far. This happens when arming motors 
    // in ArduCopter
	static uint64_t last_time = 0;
	float dt = (_systime.get_time_us() - last_time)/1000000.0f;
	if (dt > 0.005)                                              //姿态解算周期最大5ms，大于的自行修改
	{
		last_time = _systime.get_time_us();
        return;
    }
	last_time = _systime.get_time_us();

    // quat update
    AHRS_quat_update(_gyro_vector, _acc_vector, dt);


    // quat to angle
    AHRS_quat_to_angle();

}
/*姿态更新*/
void ahrs_update9()
{
	// 读取惯性传感器数据
	_ins.update9();
	
	// if the update call took more than 0.2 seconds then discard it,
    // otherwise we may move too far. This happens when arming motors 
    // in ArduCopter
	static uint64_t last_time = 0;
	float dt = (_systime.get_time_us() - last_time)/1000000.0f;
	if (dt > 0.006)                                              //姿态解算周期最大6ms，大于的自行修改
	{
		last_time = _systime.get_time_us();
        return;
    }
	last_time = _systime.get_time_us();

    // quat update
    AHRS_quat_update9(_gyro_vector, _acc_vector, _mag_vector, dt);
    
    // quat to angle
    AHRS_quat_to_angle();

}

AHRS _AHRS =
{
	ahrs_update,

};

void Test_ahrs(void)    //测试姿态解算   APM上移植过来的，智能车一般不需要解算全部的姿态
{
    uint16_t count = 0;
    uint32_t current_time;   
    uint64_t last_time;
    if(_ins.init())
    {
        printf("MPU6050 init fail ! \n");
    }
    while(1)
    {
        current_time = _systime.get_time_us() - last_time;   //当前时间减去上一个周期的开始时间
        if(current_time > 4000)  // 一个周期4ms
        {
            last_time = _systime.get_time_us();     //记住开始时间
            _AHRS.update();
            if(count++ % 200 == 0)
            printf("X  %5.2f   Y  %5.2f   Z   %5.2f  \n",ahrs_angle.x, ahrs_angle.y, ahrs_angle.z);
        }
       
    }
}
void Test_ahrs9(void)    //测试姿态解算   APM上移植过来的，智能车一般不需要解算全部的姿态
{
    uint16_t count = 0;
    uint32_t current_time;   
    uint64_t last_time;
    char txt[16];
    TFTSPI_Init();               //TFT1.8初始化  
    TFTSPI_CLS(u16BLUE);           //清屏
    LQ_init9AX();                //九轴初始化
    while(1)
    {
        current_time = _systime.get_time_us() - last_time;   //当前时间减去上一个周期的开始时间
        if(current_time > 5000)  // 一个周期4ms
        {
            last_time = _systime.get_time_us();     //记住开始时间
            ahrs_update9();
            if(count++ % 200 == 0)
            sprintf(txt, "X  %5.2f   Y  %5.2f   Z   %5.2f  \n",ahrs_angle.x, ahrs_angle.y, ahrs_angle.z);
            TFTSPI_P8X8Str(0,0,(uint8_t*)txt,u16RED,u16BLUE);
        }
       
    }
}

