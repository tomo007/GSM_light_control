#include <SoftwareSerial.h>
SoftwareSerial gsmSerial(2,3); 
int SMS_location_number;
const unsigned int MAX_INPUT = 165; // 160 characters for SMS plus a few extra
static unsigned int input_pos = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT); //Test output
  pinMode(12, OUTPUT); //Drive to relay
  Serial.begin(9600);         //Start default serial for LCD screen and debug
  gsmSerial.begin(9600); 
 GSM_Startup();     //Starts addtional serial port for GSM
  digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
}

void loop() {
  readTC35i();
  
  // put your main code here, to run repeatedly:

}
void readTC35i()
{
  static char input_line [MAX_INPUT];    //static unsigned int input_pos = 0;
  if (gsmSerial.available () > 0) //
  {
    while (gsmSerial.available () > 0)
    {
      char inByte = gsmSerial.read ();
      switch (inByte)
      {

      case '\n':   // end of text
        input_line [input_pos] = 0;  // terminating null byte

        // terminator reached! process input_line here ...
        process_data (input_line);

        // reset buffer for next time
        input_pos = 0; 
        break;

      case '\r':   // discard carriage return
        break;

      default:
        // keep adding if not full ... allow for terminating null byte
        if (input_pos < (MAX_INPUT - 1))
          input_line [input_pos++] = inByte;
        break;

      }  // end of switch
    }  // end of while incoming data
  }  // end of if incoming data
}
void process_data (char * data)
{
    //Serial.println (data); // display the data

  if(strstr(data, "+CMGR:") && strstr(data, "+3858788093"))
  { 
    // Reads the +CMGR line to check if SMS is from a known Phone number
    // This if statement could cover the whole of the process_data function
    // then only known a phone number could control the Arduoino
  }

  if(strstr(data, "smsdelete"))
  {
    delete_All_SMS();
  }

  if(strstr(data, "^SMGO: 2"))
  { // SIM card FULL
    delete_All_SMS();           // delete all SMS
  }

  if(strstr(data, "+CMTI:"))
  {    // An SMS has arrived
    char* copy = data + 12;      // Read from position 12 until a non ASCII number to get the SMS location
    SMS_location_number = (byte) atoi(copy);  // Convert the ASCII number to an int
    gsmSerial.print("AT+CMGR=");
    gsmSerial.println(SMS_location_number);  // Print the SMS in Serial Monitor
  }                                          // this SMS data will go through this process_data function again
  // any true if statements will execute

  if(strstr(data, "upali")) // If data contains Heating on
  {   
       
    
    digitalWrite(13, HIGH);
      digitalWrite(12, LOW);
 
 }
  if(strstr(data, "ugasi")) // If data contains Heating on
  {   
       
    digitalWrite(13, LOW); 
      digitalWrite(12, HIGH);
 }
 }
void delete_All_SMS()
{
  for(int i = 1; i <= 10; i++) {
    gsmSerial.print("AT+CMGD=");
    gsmSerial.println(i);
    Serial.write("deleting SMS ");
    Serial.print(i);
    delay(500);
  }
}
void GSM_Startup()
{
  gsmSerial.print("AT+CMGF=1\r");
  delay(100);
  gsmSerial.print("AT+CNMI=2,1,0,0,1\r");
  delay(100); 
  delete_All_SMS();
}// REPLACE THIS WITH RELAY CONNECTION

