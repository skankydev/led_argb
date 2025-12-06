# 💡 SkankyHome-Module

Le firmware pour les ESP32 qui font briller les trucs.

Des modules WiFi qui écoutent sagement les ordres MQTT du serveur SkankyHome et qui transforment ça en lumières RGB sur des bandes LED WS2812B. 

**Stack** : ESP32 + Arduino + MQTT + FastLED (WS2812B)

Ça se connecte au WiFi, ça cause en MQTT, ça reçoit des commandes JSON et ça fait clignoter des LEDs dans tous les sens. Simple, efficace, et ça fait u Kibrille.