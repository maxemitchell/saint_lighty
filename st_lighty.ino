/******************************************************************************
   Audio Reactive Lighting to be used at St. Patty's Day 2022 in Chicago
   Base Code from Sparkfun's Spectrum Shield Demo (Wes Furuya @ SparkFun Electronics and Toni Klopfenstein @SparkFun Electronics)
   https://github.com/sparkfun/Spectrum_Shield
   I am Max Mitchell
   March 2022
   use this at your own parties (only if I'm invited) :)
*********************************************************************************/
#include <FastLED.h>

// Declare audio pins
#define STROBE 4
#define RESET 5
#define DC_Left A0
#define DC_Right A1

// Declare LED pins
#define STRIP_1 7
#define STRIP_2 8
#define STRIP_3 9
#define STRIP_4 10
#define STRIP_5 11
#define STRIP_6 12
#define STRIP_7 13

// Define audio variables
int Frequencies_Left[7];
int Frequencies_Right[7];

// Declare LED variables
#define NUM_STRIPS 7
#define NUM_LEDS 30
CRGB leds[NUM_STRIPS][NUM_LEDS];

/********************Setup Loop*************************/
void setup()
{
    // Set spectrum Shield pin configurations
    pinMode(STROBE, OUTPUT);
    pinMode(RESET, OUTPUT);
    pinMode(DC_Left, INPUT);
    pinMode(DC_Right, INPUT);

    // Initialize Spectrum Analyzers
    digitalWrite(STROBE, LOW);
    digitalWrite(RESET, LOW);
    delay(5);

    // Initialize LEDs
    FastLED.addLeds<WS2812B, STRIP_1, GRB>(leds[0], NUM_LEDS);
    FastLED.addLeds<WS2812B, STRIP_2, GRB>(leds[1], NUM_LEDS);
    FastLED.addLeds<WS2812B, STRIP_3, GRB>(leds[2], NUM_LEDS);
    FastLED.addLeds<WS2812B, STRIP_4, GRB>(leds[3], NUM_LEDS);
    FastLED.addLeds<WS2812B, STRIP_5, GRB>(leds[4], NUM_LEDS);
    FastLED.addLeds<WS2812B, STRIP_6, GRB>(leds[5], NUM_LEDS);
    FastLED.addLeds<WS2812B, STRIP_7, GRB>(leds[6], NUM_LEDS);
}

/**************************Main Function Loop*****************************/
void loop()
{
    analyze_audio();
    light_leds();
}

/*******************Pull frquencies from Spectrum Shield********************/
void analyze_audio()
{
    digitalWrite(RESET, HIGH);
    delayMicroseconds(200);
    digitalWrite(RESET, LOW);
    delayMicroseconds(200);

    // Read frequencies for each band
    for (int freq_band = 0; freq_band < 7; freq_band++)
    {
        digitalWrite(STROBE, HIGH);
        delayMicroseconds(50);
        digitalWrite(STROBE, LOW);
        delayMicroseconds(50);

        Frequencies_Left[freq_band] = analogRead(DC_Left);
        Frequencies_Right[freq_band] = analogRead(DC_Right);
    }
}

/*****************Light Up the 7 LED Strips*****************/
void light_leds()
{
    for (int freq_band = 0; freq_band < 7; freq_band++)
    {
        int lit_leds = (Frequencies_Left[freq_band] + Frequencies_Right[freq_band]) / 2046. * NUM_LEDS;

        fill_rainbow(leds[freq_band], lit_leds, HUE_GREEN, 1);
        FastLED.show();
        fadeToBlackBy(leds[freq_band], NUM_LEDS, 40);
    }
}
