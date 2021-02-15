    const int mixing_tank_level_min =3 ; //Min allowable water height in mixing tank
    const int mixing_tanK_level_max =3; //Min allowable
    const int farm_runoff_max = 10; //max allowable
    const int freshwater_max = 10;  //max allowable height in freshwater tank
    const int freshwater_min = 10; //Min allowable height in freshwater tank
    const int excess_water_max = 10; //Max. allowable water height in freshwater tank
    const int nutrient_tank_min = 10; //min. allow. height in nutrient/pH buckets

    //values are in seconds
    const int sprinkler_time_ON = 15; //duration of each farm spraying
    const int sprinkler_time_OFF = 30; //rest time b/w each sprayinh
  
    const double supply_pressure_min = 60*6895; //min. allow. in line [PSI to Pa]
    const double supply_pressure_max = 120*6895; //[Pa]

    const int water_ec_min = 3; //min. allow. EC (nutrient content in mixing tank)
    const int water_ec_max = 3; //Max allow

    const int water_PH_min = 3; //min. PH allowed in mixing tank

    const int optim_light_intensity_1 = 3; //Optim. value for plants on top shelf
    const int optim_light_intensity_2 = 3; //optim. val for plants on bottom shelf

    //variables to read in data from sensors
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
 setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    
    pinMode(Read_Supply_Pressure,INPUT); //Read Supply pressure
    pinMode(Read_Excess_water,INPUT); //Read excess water sensor
    pinMode(Read_Farm_Runoff,INPUT); //read farm runoff
    pinMode(Read_Freshwater,INPUT); //read freshwater sensor
    pinMode(Read_Nutrient_Tank_A,INPUT); //read info from tank A
    pinMode(Read_Nutrient_Tank_B,INPUT); //read data from tank B
    pinMode(Read_PH_Tank,INPUT); //Read info. from ph tank
    pinMode(Read_Mixing_Tank,INPUT); //Read info. from mixing tank
    pinMode(Read_Water_EC,INPUT); //Read info. from Water nutrients
    pinMode(Read_Ambient_Temp, INPUT);
    pinMode(Show_Webcam_Livestream, INPUT);
    pinMode(Track_Plant1, INPUT);
    pinMode(Track_Plant2, INPUT);
    pinMode(Track_Plant3, INPUT);
    pinMode(Track_Plant4, INPUT);
    

    pinMode(Actuate_Nutrient_Pump, OUTPUT);
    pinMode(Actuate_3way_Valve,OUTPUT);
    pinMode(Actuate_return_pump,OUTPUT);
    pinMode(Actuate_sprinkler_valve,OUTPUT);
    pinMode(Actuate_Supply_pump,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int runtime;
  int flag;
  Supply_pressure = analogRead(Read_Supply_Pressure);
  Excess_Water = analogRead(Read_Excess_Water);
  Farm_Runoff = analogRead(Read_Farm_Runoff);
  Freshwater = analogRead(Read_Freshwater);
  Nutr_A = analogRead(Read_Nutrient_Tank_A);
  Nutr_B = analogRead(Read_Nutrient_Tank_B);
  ph_tank = analogRead(Read_PH_Tank);
  Mixing_tank = analogRead(Read_Mixing_Tank);
  Water_EC = analogRead(Read_Water_EC);
  
  //Supply Line
  while(1){ //Infinite loop to keep it running seperate from the system
    digitalWrite(Actuate_sprinkler_valve, High); //open sprinkler valve
    runtime = sprinkler_time_ON; 
    digitalWrite(Actuate_sprinkler_valve, Low); //close valve
    runtime = sprinkler_time_OFF;
  }
  
  if(Supply_Pressure < supply_pressure_min){ //Pressure is less than min. allow. pressure
    while(Supply_Pressure < supply_pressure_max){ //Run untill presure goes over min. pressure
          digitalWrite(Actuate_Supply_pump, High); //Open supply valve
          Supply_Pressure = analogRead(Read_Supply_Pressure); //Re-read data
          if(Supply_Pressure == supply_presure_max){ //if pressure reachs max then close valve and break.
            digitalWrite(Actuate_Supply_pump, Low);
            break;
          }
    }
  }
  if(Excess_Water > excess_water_max){ //If Excess water is above acceptable level
    Serial.println("Please empty excess water storage tank");
  }

  //Return Line
  if(Farm_Runoff >= farm_runoff_max){ //If runoff tank is full
    while(Farm_Runoff >= farm_runoff_min) // Run till its normal
          digitalWrite(Actuate_return_pump, High); //Open return valve (drain runoff tank)
          Farm_Runoff = analogRead(Read_Farm_Runoff);
  }
  digitalWrite(Actuate_return_pump, Low);
  if(Freshwater < freshwater_min){
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
  int avg_weight;
  for(int i = 0; i < sizeof(Plant);i++){
    avg_weight+ = 0.25*Plant[i];
  }
  //Can't Figure out how to alter light intensity
  
  //Mixing Tank
  if(Mixing_tank < mixing_tank_level_min){ //Mixing tank needs more water
    while(flag = 1){ //Run until condition remains true
    if(Farm_Runoff > farm_runoff_min){
      digitalWrite(Actuate_return_pump, High);
        if(Farm_Runoff == farm_runoff_max){ //Turn off pump
          digitalWrite(Actuate_return_pump, Low);
          flag = 0;
        }
    }
    else if(Freshwater > freshwater_min){
      digitalWrite(Actuate_3way_valve, High);
      delay(5000);
      digitalWrite(Actuate_return_pump, High);
    }
    digitalWrite(Actuate_3way_valve, Low); //Turn off pumps since they're not needed
    digitalWrite(Actuate_return_pump, Low);
    else{
      Serial.println("Please refill freshwater storage tank");
    }
  }
  }
  else if(Mixing_tank > mixing_tank_level_max){ //Too Much water
    if(Excess_water < excess_water_max){
      digitalWrite(Actuate_3way_valve, High);
      delay(5000);
      digitalWrite(Actuate_supply_pump, High);
    }
    else{
      Serial.println("Please empty excess water storage tank");
    }
  }
  while(Water_EC <= water_EC_min){ //Need more nutrients
    digitalWrite(Select_Hose_Port_X, High);
    delay(5000);
    digitalWrite(Actuate_Nutrient_Pump, High);
    }
  
 
  while(Water_EC > water_EC_max){
    Water_EC = analogRead(Read_Water_EC);
    if(Freshwater > freshwater_level_min){
      digitalWrite(Actuate_3way_valve, High);
      delay(5000);
      digitalWrite(Actuate_return_pump, High);
    }
    digitalWrite(Actuate_return_pump, Low);
  }
    else{
      Serial.println("Please refill freshwater storage tank");
    }
  }
  flag = 1;
  while(ph_tank > water_PH_main){
    analogRead(Read_PH_Tank)
    digitalWrite(Select_Hose_Port_Y, High);
    delay(5000);
    digitalWrite(Actuate_Nutrient_Pump, High);
    }
    digitalWrite(Actuate_Nutrient_Pump, Low);
}
