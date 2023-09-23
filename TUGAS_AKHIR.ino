
void setup()
{
   
   pinMode(pwm1, OUTPUT);
   pinMode(pwm2, OUTPUT);
   pinMode(pwm3, OUTPUT);
   pinMode(pwm4, OUTPUT);
   pinMode(key1, INPUT_PULLUP);
   pinMode(key2, INPUT_PULLUP);
   pinMode(key3, INPUT_PULLUP);
   pinMode(key4, INPUT_PULLUP);
   lcd.begin (16,2);
   Serial.begin(9600);
   mySerial.begin(9600);
   scale.set_scale();
   scale.tare();
   
}

void loop() 
{
  b = 0;
  w = 4;
  menu();
  cek_berat();
  lcd.clear();
  while ( w == 4)
  {
  read_sensor_values();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("OTW");
  if (error == 100)
  {
    analogWrite(pwm2, 0); 
    analogWrite(pwm1, 0);
    analogWrite(pwm4, 0);
    analogWrite(pwm3, 0);
    delay(1000);
    analogWrite(pwm2, 0); 
    analogWrite(pwm1, 30);
    analogWrite(pwm4, 0);
    analogWrite(pwm3, 30);
    delay(600);
    analogWrite(pwm2, 0); 
    analogWrite(pwm1, 0);
    analogWrite(pwm4, 0);
    analogWrite(pwm3, 0);
    delay(700);
    cek_barcode();
  }
  else
  {
  calculate_pid();
  motor_control();
  }
  }
}

void read_sensor_values()
{
  data1 = analogRead(pin0); 
  data2 = analogRead(pin1);
  data3 = analogRead(pin2);
  data4 = analogRead(pin3);
  data5 = analogRead(pin4);
  data6 = analogRead(pin5);

 
  if      ((data1 > 901) && (data2 > 891) && (data3 > 920) && (data4 > 894) && (data5 > 885) && (data6 <= 902))
    error = 3;
  else if ((data1 > 901) && (data2 > 891) && (data3 > 920) && (data4 > 894) && (data5 <= 885) && (data6 <= 902))
    error = 2;
  else if ((data1 > 901) && (data2 > 891) && (data3 > 920) && (data4 <= 894) && (data5 <= 885) && (data6 > 902))
    error = 1; 
  else if ((data1 > 901) && (data2 > 891) && (data3 <= 920) && (data4 <= 894) && (data5 > 885) && (data6 > 902))
    error = 0;
  else if ((data1 > 901) && (data2 <= 891) && (data3 <= 920) && (data4 > 894) && (data5 > 885) && (data6 > 902))
    error = -1;
  else if ((data1 <= 901) && (data2 <= 891) && (data3 > 920) && (data4 > 894) && (data5 > 885) && (data6 > 902))
    error = -2;
  else if ((data1 <= 901) && (data2 > 891) && (data3 > 920) && (data4 > 894) && (data5 > 885) && (data6 > 902))
    error = -3 ;
  else if ((data1 <= 901) && (data2 <= 891) && (data3 <= 920) && (data4 <= 894) && (data5 <= 885)&& (data6 <= 902)) 
    error = 100;
}

void calculate_pid()
{
    P = error;
    I = I + error;
    D = error - previous_error;

    PID_value = (Kp * P) + ((Ki * I)*ts) + ((Kd * D)/ts);
    previous_error = error;
}

void motor_control()
{
  
  left_motor_speed = initial_motor_speed - PID_value;
  right_motor_speed = initial_motor_speed + PID_value;

  
  left_motor_speed = constrain(left_motor_speed, 0, 255);
  right_motor_speed = constrain(right_motor_speed, 0, 255);

  

  
  forward();
}

void forward()
{
  
  analogWrite(pwm2, 0); 
  analogWrite(pwm1, right_motor_speed);
  analogWrite(pwm4, 0);
  analogWrite(pwm3, left_motor_speed);
}

void menu()
{
  tombol_utama();
  if(x == 0)
  { 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alamat Meja       ");
    if(!key4S)
    {
      menu_sub();
    }
  }
  if(x == 1)
  { 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Antar       ");
    if(!key4S)
    {
      Serial.println("Done");
      return;
    }
  }
  menu();
}

void tombol_utama()
{
  key1S = digitalRead(key1);
  key2S = digitalRead(key2);
  key3S = digitalRead(key3);
  key4S = digitalRead(key4);
  if(!key1S)
  {
    x++;
    if(x > 1)
    {
      x = 0;
    }
  }
  if(!key2S)
  {
    x--;
    if(x < 0)
    {
      x = 1;
    }
  }
}

void menu_sub()
{     
  int key3S = digitalRead(key3);
  tombol_sub();
  lcd.setCursor(0,0);
  lcd.print("Pilih Meja : ");
      
  if (m == 0)
  {
    y = 'P';
    r = 'Q';
    s = 'R';
    t = 'O';
    lcd.clear();
    lcd.setCursor(13, 0);
    lcd.print(m+1);
  }
  else if (m == 1)
  {     
    y = 'Q';
    r = 'P';
    s = 'R';
    t = 'O';
    lcd.clear();
    lcd.setCursor(13, 0);
    lcd.print(m+1);   
  }
  else if (m == 2)
  {      
    y = 'R';
    r = 'P';
    s = 'Q';
    t = 'O';
    lcd.clear();
    lcd.setCursor(13, 0);
    lcd.print(m+1);      
  }
  else if (m == 3)
  {      
    y = 'O';
    r = 'P';
    s = 'Q';
    t = 'R';
    lcd.clear();
    lcd.setCursor(13, 0);
    lcd.print(m+1);      
  }  
  if(!key3S)
  {
    return;
  }
  menu_sub();    
}

void tombol_sub()
{
  int key1S = digitalRead(key1);
  int key2S = digitalRead(key2);
  if(!key1S)
  {
    m++;
    if(m > 3)
    {
      m = 0;
    }
  }
  
  if(!key2S)
  {
    m--;
    if(m < 0)
    {
      m = 3;
    }
  }
}

void cek_berat()
{
  scale.set_scale(calibration_factor); 
  z = scale.get_units();
  kg = z*(1);    
  if(kg >= 0.800)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ready in");
    lcd.setCursor(0, 1);
    lcd.print("3 Second");
    analogWrite(pwm2, 0); 
    analogWrite(pwm1, 0);
    analogWrite(pwm4, 0);
    analogWrite(pwm3, 0);
    delay(3000);
    return;
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No Food");
    analogWrite(pwm2, 0); 
    analogWrite(pwm1, 0);
    analogWrite(pwm4, 0);
    analogWrite(pwm3, 0);
    
    cek_berat();
  }
}

void cek_barcode()
{ 
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("cek alamat");
  if (mySerial.available())
  {
    j = mySerial.read();
    Serial.write(j);
  }
  
  delay(1000);
  
  if (j == y)
  {  
    read_sensor_values();
    delay(500);
    lcd.clear();
    cek_berat_akhir();
  }
  else if (j == 'K') 
  {
    
    analogWrite(pwm2, 0); 
    analogWrite(pwm1, 0);
    analogWrite(pwm4, 0);
    analogWrite(pwm3, 0);
    delay(1000);
    w = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("masuk dapur");
    delay(3000);
    return;
  }
  else if (j == r || j == s || j == t)
  {
    read_sensor_values();
    delay(1000);
    return;
  }
  if (b == 1)
  {
    read_sensor_values();
    delay(1000);
    lcd.clear();
    b = 0;
    return;
  }
  cek_barcode();
}

void cek_berat_akhir()
{
  scale.tare();
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ambil makanannya");
  analogWrite(pwm2, 0); 
  analogWrite(pwm1, 0);
  analogWrite(pwm4, 0);
  analogWrite(pwm3, 0);
  delay(1000);
  
  
  scale.set_scale(calibration_factor); 
  z = scale.get_units();
  kg = z*(1);
  if(kg <= -0.800)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("The Robot Will");
    lcd.setCursor(0, 1);
    lcd.print("Move In 5 Second");
    b = 1;
    delay(5000);
    lcd.clear();
    // delay
    return;
  }
  else
  {
    cek_berat_akhir();
  }
}
