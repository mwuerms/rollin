/**
 * Martin Egli
 * 2025-06-28
 * bldc motor module
 */

#include "bldc_motor.h"
#include <math.h>

uint16_t bldc_motor_init(bldc_motor_t *m, bldc_driver_t *d, angle_sens_t *as) {
    if(m == NULL) {
        // error, invalid motor
        return false;
    }
    if(d == NULL) {
        // error, invalid bldc driver, this is mandatory
        return false;
    }
    m->d = d;
    if(as == NULL) {
        // error, invalid angle sensor, this is mandatory
        return false;
    }
	m->as = as;

    m->ctrl.status &= ~BLDC_MOTOR_STATUS_ENABLED;
    m->ctrl.type = BLDC_MOTOR_CTRL_TYPE_VELOCITY_OPENLOOP;
    m->limit.voltage = 0.0f;
    m->pid.kp = 0.0f;
    m->pid.ki = 0.0f;
    m->pid.kd = 0.0f;
    return true;
}

uint16_t bldc_motor_set_motor_parameters(bldc_motor_t *m, uint16_t pp, float kv, float coil_res) {
    if(m == NULL) {
        // error, invalid motor
        return false;
    }
    m->motor.nb_pole_pairs = pp;
    m->motor.kv = kv;
    m->motor.coil_resistance = coil_res;
    return true;
}

uint16_t bldc_motor_set_ctrl_type(bldc_motor_t *m, uint16_t ctype) {
	if(m == NULL) {
        // error, invalid motor
        return false;
    }
    m->ctrl.type = ctype;
    return true;
}

uint16_t bldc_motor_set_pid(bldc_motor_t *m, float kp, float ki, float kd) {
	if(m == NULL) {
        // error, invalid motor
        return false;
    }
    m->pid.kp = kp;
    m->pid.ki = ki;
    m->pid.kd = kd;
    return true;
}

uint16_t bldc_motor_set_voltage_limit(bldc_motor_t *m, float voltage_limit) {
	if(m == NULL) {
        // error, invalid motor
        return false;
    }
    m->limit.voltage = voltage_limit;
    return true;
}

uint16_t bldc_motor_set_target_speed(bldc_motor_t *m, float speed) {
	if(m == NULL) {
        // error, invalid motor
        return false;
    }
    m->target.speed_rot_s = speed;
    m->target.angle_deg_s = 360.0f * speed;
    return true;
}

uint16_t bldc_motor_set_target_angle_deg(bldc_motor_t *m, float angle_deg) {
    if(m == NULL) {
        // error, invalid motor
        return false;
    }
    m->target.angle_deg = angle_deg;
    return true;
}

/* set template
uint16_t bldc_motor_set_target_x(bldc_motor_t *m, float x) {
    if(m == NULL) {
        // error, invalid motor
        return false;
    }
    m-> = x;
    return true;
}
*/

uint16_t bldc_motor_enable(bldc_motor_t *m) {
    if(m == NULL) {
        // error, invalid motor
        return false;
    }
    if(bldc_driver_enable(m->d) == false) {
        // error, could not enable bldc driver
        return false;
    }
    // OK, motor + driver enabled
    m->ctrl.status |= BLDC_MOTOR_STATUS_ENABLED;
    return true;
    
}
uint16_t bldc_motor_disable(bldc_motor_t *m) {
    if(m == NULL) {
        // error, invalid motor
        return false;
    }
    m->ctrl.status &= ~BLDC_MOTOR_STATUS_ENABLED;
    return bldc_driver_disable(m->d);
}


float dbg_uu = 0.0f;
float dbg_uv = 0.0f;
float dbg_uw = 0.0f;
/*
float statangle_u =   0.0f;
float statangle_v = 120.0f;
float statangle_w = 240.0f;
float statangle_inc = 1.0f;
*/
uint16_t statangle_u = 0;
uint16_t statangle_v = 0;
uint16_t statangle_w = 0;
uint16_t statangle_inc = 1;

static float sinf_lut[] = {0, 0.0174524064372835, 0.034899496702501, 0.0523359562429438, 0.0697564737441253, 0.0871557427476582, 0.104528463267653, 0.121869343405147, 0.139173100960065, 0.156434465040231, 0.17364817766693, 0.190808995376545, 0.207911690817759, 0.224951054343865, 0.241921895599668, 0.258819045102521, 0.275637355816999, 0.292371704722737, 0.309016994374947, 0.325568154457157, 0.342020143325669, 0.3583679495453, 0.374606593415912, 0.390731128489274, 0.4067366430758, 0.422618261740699, 0.438371146789077, 0.453990499739547, 0.469471562785891, 0.484809620246337, 0.5, 0.515038074910054, 0.529919264233205, 0.544639035015027, 0.559192903470747, 0.573576436351046, 0.587785252292473, 0.601815023152048, 0.615661475325658, 0.629320391049837, 0.642787609686539, 0.656059028990507, 0.669130606358858, 0.681998360062498, 0.694658370458997, 0.707106781186547, 0.719339800338651, 0.73135370161917, 0.743144825477394, 0.754709580222772, 0.766044443118978, 0.777145961456971, 0.788010753606722, 0.798635510047293, 0.809016994374947, 0.819152044288992, 0.829037572555042, 0.838670567945424, 0.848048096156426, 0.857167300702112, 0.866025403784439, 0.874619707139396, 0.882947592858927, 0.891006524188368, 0.898794046299167, 0.90630778703665, 0.913545457642601, 0.92050485345244, 0.927183854566787, 0.933580426497202, 0.939692620785908, 0.945518575599317, 0.951056516295153, 0.956304755963035, 0.961261695938319, 0.965925826289068, 0.970295726275996, 0.974370064785235, 0.978147600733806, 0.981627183447664, 0.984807753012208, 0.987688340595138, 0.99026806874157, 0.992546151641322, 0.994521895368273, 0.996194698091746, 0.997564050259824, 0.998629534754574, 0.999390827019096, 0.999847695156391, 1, 0.999847695156391, 0.999390827019096, 0.998629534754574, 0.997564050259824, 0.996194698091746, 0.994521895368273, 0.992546151641322, 0.99026806874157, 0.987688340595138, 0.984807753012208, 0.981627183447664, 0.978147600733806, 0.974370064785235, 0.970295726275996, 0.965925826289068, 0.961261695938319, 0.956304755963036, 0.951056516295154, 0.945518575599317, 0.939692620785908, 0.933580426497202, 0.927183854566787, 0.92050485345244, 0.913545457642601, 0.90630778703665, 0.898794046299167, 0.891006524188368, 0.882947592858927, 0.874619707139396, 0.866025403784439, 0.857167300702112, 0.848048096156426, 0.838670567945424, 0.829037572555042, 0.819152044288992, 0.809016994374947, 0.798635510047293, 0.788010753606722, 0.777145961456971, 0.766044443118978, 0.754709580222772, 0.743144825477394, 0.731353701619171, 0.719339800338651, 0.707106781186548, 0.694658370458997, 0.681998360062499, 0.669130606358858, 0.656059028990507, 0.642787609686539, 0.629320391049837, 0.615661475325658, 0.601815023152049, 0.587785252292473, 0.573576436351046, 0.559192903470747, 0.544639035015027, 0.529919264233205, 0.515038074910054, 0.5, 0.484809620246337, 0.469471562785891, 0.453990499739547, 0.438371146789078, 0.422618261740699, 0.4067366430758, 0.390731128489274, 0.374606593415912, 0.358367949545301, 0.342020143325669, 0.325568154457157, 0.309016994374947, 0.292371704722737, 0.275637355816999, 0.258819045102521, 0.241921895599668, 0.224951054343865, 0.207911690817759, 0.190808995376545, 0.173648177666931, 0.156434465040231, 0.139173100960065, 0.121869343405148, 0.104528463267654, 0.0871557427476582, 0.0697564737441255, 0.0523359562429443, 0.0348994967025011, 0.0174524064372834, 1.22464679914735E-16, -0.0174524064372832, -0.0348994967025009, -0.0523359562429436, -0.0697564737441248, -0.0871557427476579, -0.104528463267653, -0.121869343405148, -0.139173100960066, -0.156434465040231, -0.17364817766693, -0.190808995376545, -0.207911690817759, -0.224951054343865, -0.241921895599667, -0.25881904510252, -0.275637355816999, -0.292371704722737, -0.309016994374948, -0.325568154457157, -0.342020143325669, -0.3583679495453, -0.374606593415912, -0.390731128489274, -0.4067366430758, -0.422618261740699, -0.438371146789077, -0.453990499739546, -0.469471562785891, -0.484809620246337, -0.5, -0.515038074910054, -0.529919264233205, -0.544639035015027, -0.559192903470747, -0.573576436351046, -0.587785252292473, -0.601815023152048, -0.615661475325658, -0.629320391049837, -0.642787609686539, -0.656059028990507, -0.669130606358858, -0.681998360062498, -0.694658370458997, -0.707106781186547, -0.719339800338651, -0.73135370161917, -0.743144825477394, -0.754709580222772, -0.766044443118978, -0.777145961456971, -0.788010753606722, -0.798635510047293, -0.809016994374947, -0.819152044288992, -0.829037572555041, -0.838670567945424, -0.848048096156426, -0.857167300702112, -0.866025403784438, -0.874619707139395, -0.882947592858927, -0.891006524188368, -0.898794046299167, -0.90630778703665, -0.913545457642601, -0.92050485345244, -0.927183854566787, -0.933580426497202, -0.939692620785908, -0.945518575599317, -0.951056516295153, -0.956304755963035, -0.961261695938319, -0.965925826289068, -0.970295726275996, -0.974370064785235, -0.978147600733806, -0.981627183447664, -0.984807753012208, -0.987688340595138, -0.99026806874157, -0.992546151641322, -0.994521895368273, -0.996194698091746, -0.997564050259824, -0.998629534754574, -0.999390827019096, -0.999847695156391, -1, -0.999847695156391, -0.999390827019096, -0.998629534754574, -0.997564050259824, -0.996194698091746, -0.994521895368273, -0.992546151641322, -0.99026806874157, -0.987688340595138, -0.984807753012208, -0.981627183447664, -0.978147600733806, -0.974370064785235, -0.970295726275996, -0.965925826289068, -0.961261695938319, -0.956304755963035, -0.951056516295154, -0.945518575599317, -0.939692620785908, -0.933580426497202, -0.927183854566787, -0.920504853452441, -0.913545457642601, -0.90630778703665, -0.898794046299167, -0.891006524188368, -0.882947592858927, -0.874619707139396, -0.866025403784439, -0.857167300702112, -0.848048096156426, -0.838670567945424, -0.829037572555042, -0.819152044288992, -0.809016994374947, -0.798635510047293, -0.788010753606722, -0.777145961456971, -0.766044443118978, -0.754709580222772, -0.743144825477394, -0.73135370161917, -0.719339800338651, -0.707106781186548, -0.694658370458998, -0.681998360062499, -0.669130606358859, -0.656059028990507, -0.64278760968654, -0.629320391049837, -0.615661475325658, -0.601815023152048, -0.587785252292473, -0.573576436351047, -0.559192903470747, -0.544639035015027, -0.529919264233205, -0.515038074910054, -0.5, -0.484809620246338, -0.469471562785891, -0.453990499739547, -0.438371146789078, -0.422618261740699, -0.4067366430758, -0.390731128489274, -0.374606593415912, -0.358367949545301, -0.342020143325669, -0.325568154457157, -0.309016994374948, -0.292371704722737, -0.275637355816999, -0.258819045102521, -0.241921895599668, -0.224951054343865, -0.20791169081776, -0.190808995376545, -0.17364817766693, -0.156434465040231, -0.139173100960066, -0.121869343405148, -0.104528463267654, -0.0871557427476583, -0.0697564737441256, -0.0523359562429435, -0.0348994967025008, -0.0174524064372836};

static uint16_t bldc_motor_velocity_openloop(bldc_motor_t *m, float dt) {

    m->calc.shaft_angle_deg = m->target.angle_deg_s * dt + m->calc.shaft_angle_deg_old; // next shaft angle
    m->calc.el_angle_deg = m->calc.shaft_angle_deg * m->motor.nb_pole_pairs;// next electrical angle
    float delta_el_ang = m->calc.el_angle_deg - m->calc.el_angle_deg_old;
    statangle_inc = (uint16_t)delta_el_ang;

    m->calc.shaft_angle_deg_old = m->calc.shaft_angle_deg;
    m->calc.el_angle_deg_old = m->calc.el_angle_deg;
    
    statangle_u = (statangle_u + statangle_inc) % 360;
    statangle_v = (statangle_u + 120) % 360;
    statangle_w = (statangle_u + 240) % 360;

    float uu = 0.5f + 0.5f * m->set.voltage_q * sinf_lut[statangle_u];
	float uv = 0.5f + 0.5f * m->set.voltage_q * sinf_lut[statangle_v];
	float uw = 0.5f + 0.5f * m->set.voltage_q * sinf_lut[statangle_w];

    bldc_driver_set_phase_voltages(m->d, uu, uv, uw);
    dbg_uu = uu;
    dbg_uv = uv;
    dbg_uw = uw;
    return true;
}

static uint16_t bldc_motor_angle_openloop(bldc_motor_t *m, float dt) {
    return 0;
}

static uint16_t bldc_motor_angle_velocity_pid(bldc_motor_t *m, float dt) {
	float error = m->target.angle_deg_s - m->current.angle_deg_s;

	// out = kp * err + ki * err * dt + old_i + kd * err / dt;
	m->pid.integrator += m->pid.ki * error * dt;
	// limit
	if(m->pid.integrator > +m->limit.voltage) m->pid.integrator = +m->limit.voltage;
	if(m->pid.integrator < -m->limit.voltage) m->pid.integrator = -m->limit.voltage;

	m->set.voltage_q = m->pid.kp * error + m->pid.integrator;
	if(m->set.voltage_q > +m->limit.voltage) m->set.voltage_q = +m->limit.voltage;
	if(m->set.voltage_q < -m->limit.voltage) m->set.voltage_q = -m->limit.voltage;
}

uint16_t bldc_motor_move(bldc_motor_t *m, float dt) {
    if(m == NULL) {
        // error, invalid motor
        return false;
    }
    if((m->ctrl.status & BLDC_MOTOR_STATUS_ENABLED) == 0) {
        // is disabled, stop here
        return false;
    }
    // get angle
    if(angle_sensor_get(m->as) == false) {
        // error, no valid angle value
    	return false;
    }
    m->current.angle_deg = m->as->angle_deg;

    // calc angle velocity
    m->current.delta_angle_deg = (m->current.angle_deg - m->current.angle_deg_old);
    // limit
    if(m->current.delta_angle_deg >  180.0f) m->current.delta_angle_deg -= 360.0f;
	if(m->current.delta_angle_deg < -180.0f) m->current.delta_angle_deg += 360.0f;
	m->current.angle_deg_s = m->current.delta_angle_deg / dt;
    m->current.angle_deg_old = m->current.angle_deg;

    // process PID
    //m->set.voltage_q = 0.40f;
    //bldc_motor_angle_velocity_pid(m, dt);

    // calculate phase voltages u, v, w, each 120° apart
    /*float angle_rad = (m->current.angle_deg * 2*M_PI) / 360.0f;
    float uu = 0.5f + 0.5f * m->set.voltage_q * sinf(angle_rad);
    float uv = 0.5f + 0.5f * m->set.voltage_q * sinf(angle_rad - ((120.0f * 2*M_PI) / 360.0f));
    float uw = 0.5f + 0.5f * m->set.voltage_q * sinf(angle_rad - ((240.0f * 2*M_PI) / 360.0f));*/
#if 0
    float angle_rad;
    angle_rad = ((statangle_u)* 2*M_PI) / 360.0f;
    //if(angle_rad > (2*M_PI)) angle_rad -= (2*M_PI);
	float uu = 0.5f + 0.5f * m->set.voltage_q * sinf(angle_rad);
	angle_rad = ((statangle_v)* 2*M_PI) / 360.0f;
	//if(angle_rad > (2*M_PI)) angle_rad -= (2*M_PI);
	float uv = 0.5f + 0.5f * m->set.voltage_q * sinf(angle_rad);
	angle_rad = ((statangle_w)* 2*M_PI) / 360.0f;
	//if(angle_rad > (2*M_PI)) angle_rad -= (2*M_PI);
	float uw = 0.5f + 0.5f * m->set.voltage_q * sinf(angle_rad);

	statangle_u += statangle_inc;
    if(statangle_u >= 360.0f) statangle_u = 0.0f;
	statangle_v += statangle_inc;
    if(statangle_v >= 360.0f) statangle_v = 0.0f;
	statangle_w += statangle_inc;
    if(statangle_w >= 360.0f) statangle_w = 0.0f;
#endif
    switch(m->ctrl.type) {
        case BLDC_MOTOR_CTRL_TYPE_VELOCITY_OPENLOOP: 
            bldc_motor_velocity_openloop(m, dt);
            break;
        case BLDC_MOTOR_CTRL_TYPE_ANGLE_OPENLOOP: 
            bldc_motor_angle_openloop(m, dt);
            break;
    }
    
    //dbg_uw = angle_rad;

    // OK
    return true;
}
