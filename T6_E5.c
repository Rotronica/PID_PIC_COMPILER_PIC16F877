#INCLUDE <16F877.h>
#device adc=10
#use delay(clock=4000000)        
#fuses XT,NOWDT
#BYTE TRISC = 0x87                   
                                      
void main(){
int16 valor;                  //lectura de temperatura
int16 control;                //valor del PWM
float a,b,c;                  //constantes del PID
float temp_limit;            //temperatura a alcanzar
float rT,eT,iT,dT,yT,uT,iT0,eT0,iT_1,eT_1;     //variables de ecuaciones            
float max,min;               //l�mites m�ximo y m�nimo de control.
                                
min=0.0;                    //inicializaci�n variables 
max=1000.0;
iT_1=0.0;
eT_1=0.0;
a=0.1243;                     //constantes del PID 
b=0.0062;
c=0.6215;
temp_limit=500.0;          //Temperatura a alcanzar 
TRISC=0;
setup_timer_2(t2_div_by_4,249,1);     //periodo de la se�al PWM a 1ms  
setup_ccp1(ccp_pwm);                  //M�dulo CCP a modo PWM
setup_adc_ports(all_analog);         //Puerto A anal�gico
setup_adc(ADC_CLOCK_INTERNAL);       //reloj convertidor AD interno
set_adc_channel(0);                  //Lectura por el canal 0
while(1) {
                                  
  valor=read_adc();                 //Lectura de la temperatura
  yT=valor*5000.0/1024.0;           //conversi�n a mV (0.25V a 250mV)
  rT=temp_limit;                    
  eT=rT-yT;                             //C�lculo error        
  iT=b*eT+iT0;                         //C�lculo del t�rmino integral
  dT=c*(eT-eT0);                       //C�lculo del t�rmino derivativo 
  uT=iT+a*eT+dT;                      //C�lculo de la salida PID
  if (uT>max) {        //Salida PID si es mayor que el MAX 
     uT=max;}                          
  else {
     if (uT<min){      //Salida PID si es menor que el MIN 
        uT=min;}                           
        }                             
  control=uT;                     //Transferencia de salida PID a se�al PWM
  set_pwm1_duty(control);
  iT0=iT;                        //Guardar variables
  eT0=eT;
                                                                 
  delay_ms(100);                //Tiempo de muestreo
  }
}

