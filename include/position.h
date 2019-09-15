class Position
{
    int switch_pin;
    bool current_state;
    unsigned long time_since_change;
    int lights_positions[4];
    const char* status_name;
    unsigned long prev;
    unsigned long now = millis();
    bool counting = false;

    public:
    Position(const char name[], int pin, bool state, int* lights)
    {
      status_name = name;

      switch_pin = pin;
      pinMode(switch_pin, INPUT_PULLUP);

      current_state = state;
      for (int i = 0; i < 4; i++)
      {
        lights_positions[i] = lights[i];
      }
    }
    
    bool is_active()
    {
      if(!(digitalRead(switch_pin)))
      {
        //debounce button
        if(counting == false)
        {
          prev = now; //start counting to 50 millis
          counting = true;
        }
        if((now - prev) > 50)
        {
          //once 50 millis has passed
          //stop counting and return the debounced switch's value
          counting = false;
          Serial.println(status_name);
          return true;
        }  
      }
      else{
        return false;
      }
    }
    void update_leds_and_firebase()
    {
      //select appropriate color for position
      //assign color to Position.LEDs[]
      for(int i = 0; i < 12; i++)
      {
        leds[i] = CRGB::Black;
      }
      if(strcmp(status_name, "meal") == 0)
      {
        for(int i = 0; i < 4; i++)
        {
          leds[lights_positions[i]] = CRGB::Navy;
          Serial.println("Writing Green to LEDs!");
          // Firebase.setString("meal", "meal");
        }
      }
      else if(strcmp(status_name, "no meal") == 0)
      {
        for(int i = 0; i < 4; i++)
        {
          leds[lights_positions[i]] = CRGB::Crimson;
          Serial.println("Writing Red to LEDs!");
          // Firebase.setString("no_meal", "no meal");
        }
      }
      else if(strcmp(status_name, "snacks") == 0)
      {
        for(int i = 0; i < 4; i++)
        {
          FastLED.setBrightness(90);
          leds[lights_positions[i]] = CRGB::Gold;
          Serial.println("Writing Yellow to LEDs!");
          // Firebase.setString("snacks", "snack");
        }
      }
      else
      {
        for(int i = 0; i < 12; i++)
        {
          leds[i] = CRGB::Black;
        }
      }
      FastLED.show();
    }
    void setup()
    {
      pinMode(switch_pin, INPUT_PULLUP);
    }
    void loop()
    {
      now = millis();
      if (is_active())
      {
        Serial.println("Updating LEDs");
        update_leds_and_firebase();
      }
    }
};