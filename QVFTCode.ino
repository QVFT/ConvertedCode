    const int mixing_tank_level_min =3 ; //Min allowable water height in mixing tank
    const int mixing_tanK_level_max =3; //Min allowable
    const int farm_runoff_max = 10; //max allowable
    const int freshwater_max = 10;
    const int freshwater_min = 10;
    const int excess_water_max = 10;
    const int nutrient_tank_min = 10;

    const int sprinkler_time_ON = 15; //duration of each farm spraying
    const int sprinkler_time_OFF = 30;

    const double supply_pressure_min = 60*6895;
    const double supply_pressure_max = 120*6895;

    const int water_ec_min = 3;
    const int water_ec_max = 3;

    const int water_PH_max = 3;

    const int optim_light_intensity_1 = 3;
    const int optim_light_intensity_2 = 3;

    int Supply_Pressure;
    int Excess_Water;
    int Farm_Runoff;
    int Freshwater;
    int Nutr_A;
    int Nutr_B;
    int ph_tank;
    int Mixing_tank;
    int Water_EC
 setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    
    Supply_Pressure = pinMode(READ_SUPPLY_PRESSURE,INPUT);
    Excess_Water = pinMode(Read_Excess_water,INPUT);
    Farm_Runoff = pinMode(Read_Farm_Runoff,INPUT);
    Freshwater = pinMode(Read_Freshwater,INPUT);
    Nutr_A = pinMode(Read_Nutrient_Tank_A,INPUT);
    Nutr_B = pinMode(Read_Nutrient_Tank_B,INPUT);
    ph_tank = pinMode(Read_PH_Tank,INPUT);
    Mixing_tank = pinMode(Read_Mixing_Tank,INPUT);
    Water_EC = pinMode(Read_Water_EC,INPUT);
    pinMode(Read_Ambient_Temp, INPUT);
    pinMode(Show_Webcam_Livestream, INPUT);

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
  //Supply Line
  while(1){
    digitalWrite(Actuate_sprinkler_valve, High);
    runtime = sprinkler_time_ON;
    digitalWrite(Actuate_sprinkler_valve, Low);
    runtime = sprinkler_time_OFF;
  }
  if(Supply_Pressure < supply_pressure_min){
    while(Supply_Pressure < supply_pressure_max){
          digitalWrite(Actuate_Supply_pump, High);
          if(Supply_Pressure == supply_presure_max){
            digitalWrite(Actuate_Supply_pump, Low);
            break;
          }
    }
  }
  if(Excess_Water > excess_water_max){
    Serial.println("Please empty excess water storage tank");
  }

  //Return Line
  if(Farm_Runoff >= farm_runoff_max){
    while(Farm_Runoff >= farm_runoff_min)
          digitalWrite(Actuate_return_pump, High);
  }
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
  
  //Mixing Tank
  if(Mixing_tank < mixing_tank_level_min){
    while(flag = 1){
    if(Farm_Runoff > farm_runoff_min){
      digitalWrite(Actuate_return_pump, High);
        if(Farm_Runoff == farm_runoff_max){
          digitalWrite(Actuate_return_pump, Low);
          flag = 0;
        }
    }
    else if(Freshwater > freshwater_min){
      digitalWrite(Actuate_3way_valve, High);
      delay(5000);
      digitalWrite(Actuate_return_pump, High);
    }
    else{
      Serial.println("Please refill freshwater storage tank");
    }
  }
  }
  else if(Mixing_tank > mixing_tank_level_max){
    if(Excess_water < excess_water_max){
      digitalWrite(Actuate_3way_valve, High);
      delay(5000);
      digitalWrite(Actuate_supply_pump, High);
    }
    else{
      Serial.println("Please empty excess water storage tank");
    }
  }
  if(Water_EC < water_EC_min){
    digitalWrite(Select_Hose_Port_X, High);
    delay(5000);
    digitalWrite(Actuate_Nutrient_Pump, High);
      if(Water-EC >= water_EC_min){
        digitalWrite(Actuate_Nutrient_Pump, Low);
      }
  }
  
 
  else if(Water_EC > water_EC_max){
    if(Freshwater > freshwater_level_min){
      digitalWrite(Actuate_3way_valve, High);
      delay(5000);
      digitalWrite(Actuate_return_pump, High);
    }
    else{
      Serial.println("Please refill freshwater storage tank");
    }
  }
  if(ph_tank > water_PH_max){
    digitalWrite(Select_Hose_Port_Y, High);
    delay(5000);
    digitalWrite(Actuate_Nutrient_Pump, High);
  }  
}
