// const int   SENSOR_PIN    = A0;
// const float VCC           = 5.0;
// const float SENSITIVITY   = 0.066;
// const int   ADC_MAX       = 1023;
// const int   NUM_SAMPLES   = 150;

// void calibrate() {
//   long sum = 0;
//   for (int i = 0; i < NUM_SAMPLES; i++) {
//     sum += analogRead(SENSOR_PIN);
//     delay(2);
//   }
//   zeroPoint = (sum / (float)NUM_SAMPLES) * (VCC / ADC_MAX);
// }