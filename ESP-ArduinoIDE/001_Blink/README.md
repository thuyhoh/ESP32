# IO Blink

## GPIO
``` C
/**
 * @brief: 
 * @param[PIN_Num]: Pin Number
 * @param[MODE]: INPUT/OUTPUT/ANALOG
*/
pinMode(PIN_Num, MODE);

/**
 *
*/
digitalWrite(PIN_Num, level);
/**
 *
*/
digitalRead(PIN_Num);

/**
 * 
 * 
 * @param[Value]: int(0->255)
*/
analogWrite(PIN_Num, Value);

/**
 * 
 * 
 * @retval: int(0->255)
*/
analogRead(PIN_Num);
/**
 * 
 * @param[interruptPin]
*/
attachInterrupt(interruptPin, funct_ISR, MODE);
void funct_ISR(void)
{
    // interrupt handler
}

```
## Uart
``` C
/**
 *
*/
Serial.begin(Baudrate);
/**
 *
*/
Serial.println(string);/Serial.print(string);
```



