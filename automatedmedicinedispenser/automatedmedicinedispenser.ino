#define BLYNK_TEMPLATE_ID "TMPL3CvT5_zbp" 
#define BLYNK_TEMPLATE_NAME "MedicineDisTemplate" 
#include <BlynkSimpleEsp8266.h> 
#include <AccelStepper.h> // Include the AccelStepper library 

const char ssid[] = "MED_DISP"; 
const char pass[] = "yellow123"; 
const char auth[] = "uZPqUJhW9vOOLF7xwKx11y5taGLrLieO"; 

// Define the motor pins: 
#define MP1 0 // IN1 on the ULN2003 
#define MP2 4 // IN2 on the ULN2003 
#define MP3 5 // IN3 on the ULN2003 
#define MP4 16 // IN4 on the ULN2003 
#define MotorInterfaceType 8 // Define the interface type as 8 = 4 wires * step factor (2 for half step)

AccelStepper stepper = AccelStepper(MotorInterfaceType, MP1, MP3, MP2, MP4); // Define the pin sequence (IN1-IN3-IN2-IN4) 
const int SPR = 2048; // Steps per revolution 
const int ir_sensor = 14; 
const int buzzer = 13;

BlynkTimer timer; 

// This function runs every time Blynk connection is established 
BLYNK_CONNECTED() { 
  Serial.println("Connected to Blynk server!"); 
}

BLYNK_WRITE(V0) { 
  int pinValue = param.asInt(); 
  Serial.println(pinValue); 
  stepper.setMaxSpeed(1000); // Set the maximum motor speed in steps per second 
  stepper.setAcceleration(200); // Set the maximum acceleration in steps per second^2 
  stepper.moveTo(SPR / 4); // Set the target motor position (i.e., turn motor for 1/4 revolution) 
  stepper.runToPosition(); // Run the motor to the target position  
  delay(3000); 
  ESP.restart(); 
}

void setup() { 
  Serial.begin(9600); 
  Serial.println("In Setup"); 
  Serial.println("Connecting to WiFi..."); 
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, pass); 

  pinMode(buzzer, OUTPUT); 
  pinMode(ir_sensor, INPUT); 

  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print("."); 
  } 

  Serial.println("\nWiFi connected."); 
  Serial.print("IP Address: "); 
  Serial.println(WiFi.localIP()); 

  Blynk.config(auth); // Initializes Blynk, without blocking the connection 
  Blynk.connect(); 

  // Setup a function to be called every 10 seconds 
  timer.setInterval(10000L, []() { 
    if (!Blynk.connected()) { // If not connected to Blynk server 
      Serial.println("Lost connection to Blynk server. Reconnecting..."); 
      Blynk.connect(); // Non-blocking reconnect 
    } else { 
      Serial.println("Blynk is already connected"); 
    } 
  }); 
}

void loop() { 
  sense_buzzer(); 
  Blynk.run(); 
  timer.run(); 
}

void sense_buzzer() { 
  int buttonState = digitalRead(ir_sensor); 
  if (buttonState == 0) { 
    Serial.println("Buzzer started"); 
    digitalWrite(buzzer, HIGH); 
  } else { 
    Serial.println("Buzzer stopped"); 
    digitalWrite(buzzer, LOW); 
  } 
}
