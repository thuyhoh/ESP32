# <WiFi.h>
## API
``` C
/**
 * Connect Wifi
*/
WiFi.begin(ssid/*string*/, pass/*string*/);
/**
 * Check Wifi connect status
*/
WiFi.status();
/**
 * @brief: Check Wifi get local IP 
 * @retval: enum wl_status_t
*/
WiFi.localIP();
```

## Diagram
![image](wifi_diagram.png)
