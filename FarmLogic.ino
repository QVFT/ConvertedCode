
//Constants
const int mixing_tank_level_min = 5;
const int mixing_tank_level_max = 10;

const int farm_runoff_level_min = 5;
const int farm_runoff_level_max = 10;

const int freshwater_level_min = 5;
const int freshwater_level_max = 10;

const int excess_water_level_max = 10;
const int nutrient_tank_level_min = 5;

const int sprinkler_time_ON = 15;
const int sprinkler_time_OFF = 30;

const long int supply_pressure_min = 60*6895;
const long int supply_pressure_max = 120*6895;

const int water_ec_min = 2;
const int water_ec_max = 2;

const int water_PH_max = 7;

const int optim_light_intensity_1 = 10;
const int optim_light_intensity_2 = 10;

//Assigning analog pins (need more pins!)
const int read_supply_pressure = A0;
const int read_excess_water = A1;
const int read_farm_runoff = A2;
const int read_freshwater = A3;
const int read_PH = A4;
const int read_mixing_tank = A5;
const int read_Nutr_tank_A;
const int read_Nutr_tank_B;
const int read_water_EC;
const int led_1;
const int led_2;
const float get_temp;
const float plant_weight_1;
const float plant_weight_2;
const float plant_weight_3;
const float plant_weight_4;

//Assigning digital pins
const int nutrient_pump = 2;
const int valve_3way = 3;
const int return_pump = 4;
const int sprinkler_valve = 5;
const int supply_pump = 6;
const int hose_port_x = 7;
const int hose_port_y = 8;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //Setting pin modes as input for respective pins
  pinMode(read_supply_pressure, INPUT);
  pinMode(read_excess_water, INPUT);
  pinMode(read_farm_runoff, INPUT);
  pinMode(read_freshwater, INPUT);
  pinMode(read_PH, INPUT);
  pinMode(read_mixing_tank, INPUT);
  pinMode(read_Nutr_tank_A, INPUT);
  pinMode(read_Nutr_tank_B, INPUT);
  pinMode(read_water_EC, INPUT);
  pinMode(get_temp, INPUT);
  pinMode(plant_weight_1, INPUT);
  pinMode(plant_weight_2, INPUT);
  pinMode(plant_weight_3, INPUT);
  pinMode(plant_weight_4, INPUT);


  //Setting pin mode as OUTPUT for respective pins
  pinMode(nutrient_pump, OUTPUT);
  pinMode(valve_3way, OUTPUT);
  pinMode(return_pump,OUTPUT);
  pinMode(sprinkler_valve, OUTPUT);
  pinMode(supply_pump, OUTPUT);
  pinMode(hose_port_x, OUTPUT);
  pinMode(hose_port_y, OUTPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  //Not sure how to make this run seperately from the rest of the system
  digitalWrite(sprinkler_valve, HIGH);
  delay(sprinkler_time_ON);
  digitalWrite(sprinkler_valve, LOW);
  delay(sprinkler_time_OFF);

  int supply_pressure = analogRead(read_supply_pressure); //Read supply pressure
 
  if(supply_pressure < supply_pressure_min) {
    while(supply_pressure < supply_pressure_max) {
      digitalWrite(supply_pump, HIGH); //Open supply valve
      supply_pressure = analogRead(read_supply_pressure); //Read data again
    }
    digitalWrite(supply_pump, LOW); //Close valve once done
  }

  int excess_water = analogRead(read_excess_water); //read excess water
  
  if(excess_water > excess_water_level_max) {
    Serial.print("Please empty excess water storage tank.");
  }

  int farm_runoff = analogRead(read_farm_runoff); //Read farm runoff

  if(farm_runoff >= farm_runoff_level_max) {
    while(farm_runoff >= farm_runoff_level_min) {
      digitalWrite(return_pump, HIGH); //Open return valve
      farm_runoff = analogRead(read_farm_runoff); //read data again
    }
    digitalWrite(return_pump, LOW); //Close valve once done
  }

  int freshwater = analogRead(read_freshwater); //Read freshwater level

  if(freshwater < freshwater_level_min) {
    Serial.print("Please refill freshwater storage tank");
  }

  //Nutrient zone
  int Nutr_A = analogRead(read_Nutr_tank_A);
  int Nutr_B = analogRead(read_Nutr_tank_B);
  int PH_level = analogRead(read_PH);

  if(Nutr_A < nutrient_tank_level_min) {
    Serial.print("Please refill nutrient tank A");
  } else if (Nutr_B < nutrient_tank_level_min) {
    Serial.print("Please refill nutrient tank B");
  } else if ( PH_level < nutrient_tank_level_min) {
    Serial.print("Please refill PH tank.");
  }

  //Tracking average plant weight
  float avg_plant_weight = 0.25 * (analogRead(plant_weight_1) + analogRead(plant_weight_2) + analogRead(plant_weight_3) + analogRead(plant_weight_4)); //Record average plant weight

  //Alter light intensity
  analogWrite(led_1, optim_light_intensity_1); //Adjust light intensity (assumed it's a LED)
  analogWrite(led_2, optim_light_intensity_2);
  
  //Record ambient data
  float temp = analogRead(get_temp) * (0.004882814); //Converts 0 to 1023 value from analogRead to 0.0 to 5.0 (true voltage at the pin)
  float temp_C = (temp - 0.5) * 100.0; //Convert voltage value to Celcius
  
  int mixing_tank = analogRead(read_mixing_tank); //Read mixing tank level
  farm_runoff = analogRead(read_farm_runoff); //Read farm runoff levels again
  freshwater = analogRead(read_freshwater); //read freshwater levels again

  if(mixing_tank < mixing_tank_level_min) {
    if(farm_runoff > farm_runoff_level_min) {
      while(farm_runoff > farm_runoff_level_min) {
        digitalWrite(return_pump, HIGH); //Open return pumo
      }
    }
      digitalWrite(return_pump, LOW); //Close pump once done
  } else if (freshwater > freshwater_level_min) {
    digitalWrite(valve_3way, HIGH); //open 3 way valve
    delay(5000); //delay of 5 seconds
    digitalWrite(return_pump, HIGH); //open return valve

     //close valves once done
    digitalWrite(valve_3way, LOW);
    digitalWrite(return_pump, LOW);
  } else {
    Serial.print("Please refill freshwater storage tank.");
  }

  if(mixing_tank > mixing_tank_level_max) {
    excess_water = analogRead(read_excess_water); //read excess water again
    if(excess_water < excess_water_level_max) {
      digitalWrite(valve_3way, HIGH); //Open 3 way valve
      delay(5000); //Delay 5 seconds
      digitalWrite(supply_pump, HIGH); //Open supply valve
    }
    //Close valves once done
    digitalWrite(valve_3way, LOW);
    digitalWrite(return_pump, LOW);
  } else {
      Serial.print("Please empty excess water storage tank");
  }

  int water_EC = analogRead(read_water_EC); //read water EC
  
  if(water_EC < water_ec_min) { 
    digitalWrite(hose_port_x, HIGH); //Open hose port x
    delay(5000); //Delay 5 seconds
    digitalWrite(return_pump, HIGH); //Open return pump
    //Close once done
    digitalWrite(hose_port_x, LOW);
    digitalWrite(return_pump, LOW);  
      } else if (water_EC > water_ec_max) {
          if(freshwater > freshwater_level_min) {
              digitalWrite(valve_3way, HIGH); //Open 3 way valve
              delay(5000); //delay 5 seconds
              digitalWrite(return_pump, HIGH); //Open return valve
                }

          //Close valves once done
          digitalWrite(valve_3way, HIGH);
          digitalWrite(return_pump, HIGH);
      } else {
      Serial.print("Please refill freshwater storage tank.");
    }
  
  float cal_PH = analogRead(read_PH) * 5.0/1024; //Read PH voltage value
  float phvalue = 7 - (2.5 - PH_level) * (-5.436); //Convert value to display actual PH

  if(phvalue > water_PH_max) {
    digitalWrite(hose_port_y, HIGH); //Open hose port y
    delay(5000); //Delay 5 seconds
    digitalWrite(nutrient_pump, HIGH); //Open nutrient pump
  }

      //Close once done
      digitalWrite(hose_port_y, LOW);
      digitalWrite(nutrient_pump, LOW);
  
}
