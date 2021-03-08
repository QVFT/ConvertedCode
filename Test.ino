    const int mixing_tank_level_min =3 ; //Min alLOWable water height in mixing tank
    const int mixing_tank_level_max = 3; //Min alLOWable
    const int farm_runoff_max = 10; //max alLOWable
    const int farm_runoff_min = 10;
    const int freshwater_max = 10;  //max alLOWable height in freshwater tank
    const int freshwater_level_min = 10; //Min alLOWable height in freshwater tank
    const int excess_water_max = 10; //Max. alLOWable water height in freshwater tank
    const int nutrient_tank_min = 10; //min. alLOW. height in nutrient/pH buckets

    //values are in seconds
    const int sprinkler_time_ON = 1500; //duration of each farm spraying (ms)
    const int sprinkler_time_OFF = 3000; //rest time b/w each spraying (ms) 
  
    const double supply_pressure_min = 60*6895; //min. alLOW. in line [PSI to Pa]
    const double supply_pressure_max = 120*6895; //[Pa]

    const int water_EC_min = 3; //min. alLOW. EC (nutrient content in mixing tank)
    const int water_EC_max = 3; //Max alLOW

    const int water_PH_min = 3; //min. PH alLOWed in mixing tank

    const int optim_light_intensity_1 = 3; //Optim. value for plants on top shelf
    const int optim_light_intensity_2 = 3; //optim. val for plants on bottom shelf

    static int Read_Supply_pressure = A0;
    //static int A1 = A1;
    static int Read_Farm_Runoff = A2;
    static int Read_Freshwater = A3;
    static int Read_PH_Tank = A4;
    static int Read_Mixing_Tank = A5;
    int Read_Nutrient_Tank_A;
    int Read_Nutrient_Tank_B;
    int Read_Water_EC;
    const int Actuate_Nutrient_Pump = 2;
    const int Actuate_3way_Valve = 3;
    const int Actuate_return_pump = 4;
    const int Actuate_sprinkler_valve = 5;
    const int Actuate_Supply_pump = 6;
    const int Actuate_3way_valve = 7;
    const int Select_Hose_Port_X = 8;
    const int Select_Hose_Port_Y = 9  ;
    

    

    /*variables to read in data from sensors
    int Supply_Pressure;
    int Excess_Water;
    int Farm_Runoff;
    int Freshwater;
    int Nutr_A;
    int Nutr_B;
    int ph_tank;
    int Mixing_tank;
    int Water_EC;
    int Plant[100];
    int ambient_temp; */
 
void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    
    pinMode(A0,INPUT); //Read Supply pressure
    pinMode(A1,INPUT); //Read excess water sensor
    pinMode(Read_Farm_Runoff,INPUT); //read farm runoff
    pinMode(Read_Freshwater,INPUT); //read freshwater sensor
    pinMode(Read_Nutrient_Tank_A,INPUT); //read info from tank A
    pinMode(Read_Nutrient_Tank_B,INPUT); //read data from tank B
    pinMode(Read_PH_Tank,INPUT); //Read info. from ph tank
    pinMode(Read_Mixing_Tank,INPUT); //Read info. from mixing tank
    pinMode(Read_Water_EC,INPUT); //Read info. from Water nutrients
    //pinMode(Read_Ambient_Temp, INPUT);
    //pinMode(Show_Webcam_Livestream, INPUT);
    //pinMode(Track_Plant1, INPUT);
    //pinMode(Track_Plant2, INPUT);
    //pinMode(Track_Plant3, INPUT);
    //pinMode(Track_Plant4, INPUT);
    

    pinMode(Actuate_Nutrient_Pump, OUTPUT);
    pinMode(Actuate_3way_Valve,OUTPUT);
    pinMode(Actuate_return_pump,OUTPUT);
    pinMode(Actuate_sprinkler_valve,OUTPUT);
    pinMode(Actuate_Supply_pump,OUTPUT);
    //pinMode(Alter_Light_1, OUTPUT);
    //pinMode(Alter_Light_2, OUTPUT):
}

void loop() {
  // put your main code here, to run repeatedly:
  //int runtime;
  int flag;
  int Supply_Pressure = analogRead(A0);
  int Excess_water = analogRead(A1);
  int Farm_Runoff = analogRead(Read_Farm_Runoff);
  int Freshwater = analogRead(Read_Freshwater);
  const int Nutr_A = analogRead(A5);  //Read_Nutrient_Tank_A
  const int Nutr_B = analogRead(A4);  //Read_Nutrient_Tank_B
  int ph_tank = analogRead(Read_PH_Tank);
  int Mixing_tank = analogRead(Read_Mixing_Tank);
  int Water_EC = analogRead(Read_Water_EC);
  
  //Supply Line
  while(1){ //Infinite loop to keep it running seperate from the system
    digitalWrite(Actuate_sprinkler_valve, HIGH); //open sprinkler valve
    delay(sprinkler_time_ON); 
    digitalWrite(Actuate_sprinkler_valve, LOW); //close valve
    delay(sprinkler_time_OFF);
  }
  
  if(Supply_Pressure < supply_pressure_min){ //Pressure is less than min. alLOW. pressure
    while(Supply_Pressure < supply_pressure_max){ //Run untill presure goes over min. pressure
          digitalWrite(Actuate_Supply_pump, HIGH); //Open supply valve
          Supply_Pressure = analogRead(A0); //Re-read data
          if(Supply_Pressure == supply_pressure_max){ //if pressure reachs max then close valve and break.
            digitalWrite(Actuate_Supply_pump, LOW);
            break;
          }
    }
  }
  if(Excess_water > excess_water_max){ //If Excess water is above acceptable level
    Serial.println("Please empty excess water storage tank");
  }

  //Return Line
  if(Farm_Runoff >= farm_runoff_max){ //If runoff tank is full
    while(Farm_Runoff >= farm_runoff_min) // Run till its normal
          digitalWrite(Actuate_return_pump, HIGH); //Open return valve (drain runoff tank)
          Farm_Runoff = analogRead(Read_Farm_Runoff);
  }
  digitalWrite(Actuate_return_pump, LOW);
  if(Freshwater < freshwater_level_min){
    Serial.println("Please refill freshwater storage tank");
  }

  //Nutrient Zone
  if(Nutr_A < nutrient_tank_min){
    Serial.println("Please refill Nutrient tank A");
  }
  else if (Nutr_B < nutrient_tank_min){
    Serial.println("Please refill nutrient tank B");
  }
  if(ph_tank < nutrient_tank_min){
    Serial.println("Please refill PH tank");
  }
  //Growth zone
  //int avg_weight;
 // for(int i = 0; i < sizeof(Plant);i++){
   // avg_weight+ = 0.25*Plant[i];
  //}
  //Alter light intensity

  //analogWrite(Alter_Light_1, optim_light_intensity_1);
  //analogWrite(Alter_Light_2, optim_light_intensity_2);

  //ambient_temp = analogRead(Read_Ambient_Temp);
  
  //Mixing Tank
  if(Mixing_tank < mixing_tank_level_min){ //Mixing tank needs more water
    while(flag = 1){ //Run until condition remains true
    if(Farm_Runoff > farm_runoff_min){
      digitalWrite(Actuate_return_pump, HIGH);
        if(Farm_Runoff == farm_runoff_max){ //Turn off pump
          digitalWrite(Actuate_return_pump, LOW);
          flag = 0;
        }
    }
    else if(Freshwater > freshwater_level_min){
      digitalWrite(Actuate_3way_valve, HIGH);
      delay(5000);
      digitalWrite(Actuate_return_pump, HIGH);
    }
    digitalWrite(Actuate_3way_valve, LOW); //Turn off pumps since they're not needed
    digitalWrite(Actuate_return_pump, LOW);
    
  }
  }
  else{
      Serial.println("Please refill freshwater storage tank");
    }
 if(Mixing_tank > mixing_tank_level_max){ //Too Much water
    if(Excess_water < excess_water_max){
      digitalWrite(Actuate_3way_valve, HIGH);
      delay(5000);
      digitalWrite(Actuate_Supply_pump, HIGH);
    }
    else{
      Serial.println("Please empty excess water storage tank");
    }
  }
  while(Water_EC <= water_EC_min){ //Need more nutrients
    digitalWrite(Select_Hose_Port_X, HIGH);
    delay(5000);
    digitalWrite(Actuate_Nutrient_Pump, HIGH);
    }
  
 
  while(Water_EC > water_EC_max){
    Water_EC = analogRead(Read_Water_EC);
    if(Freshwater > freshwater_level_min){
      digitalWrite(Actuate_3way_valve, HIGH);
      delay(5000);
      digitalWrite(Actuate_return_pump, HIGH);
    }
    else{
      Serial.println("Please refill freshwater storage tank");
    }
     digitalWrite(Actuate_return_pump, LOW);
}
  flag = 1;
  while(ph_tank > water_PH_min){
    analogRead(Read_PH_Tank);
    digitalWrite(Select_Hose_Port_Y, HIGH);
    delay(5000);
    digitalWrite(Actuate_Nutrient_Pump, HIGH);
    }
    digitalWrite(Actuate_Nutrient_Pump, LOW);
}
