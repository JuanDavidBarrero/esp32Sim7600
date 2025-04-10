#include <Arduino.h>
#include "core.h"
#include "config.h"
#include "gps.h"
#include "HttpClientModule.h"
#include "mqttClient.h"
#include "ssl_config.h"
#include "cerc.h"
#include "smsClient.h"

const uint32_t BAUD_RATE = 115200;
const uint8_t RX_PIN = 26;
const uint8_t TX_PIN = 27;
CelularModule cellularModule(BAUD_RATE, RX_PIN, TX_PIN);
ConfigModule config(cellularModule);
// SmsClient smsClient(cellularModule);
// SslConfig sslClient(cellularModule);
GPSModule gpsClient(cellularModule);
// HttpClientModule httpclient(cellularModule);
// MqttClient mqttClient(cellularModule);

void setup()
{
  Serial.begin(115200);
  Serial.println("Inicializando SIM7600...");

  cellularModule.init();

  config.configCelular();
  const char *imei = config.getIMEI();
  Serial.print("El imei es ");
  Serial.println(imei);
  config.isReadySIM();
  config.setAPN("internet.comcel.com.co");

  delay(5000);


  Serial.println("\n\n\n=============================  GPS EXAMPLE =============================\r\n\n");


  gpsClient.activeGPS();

  if(gpsClient.isActiveGPS()){

    Coordinates dataGPS;
    Serial.println("\r\nEl gps esta activo\r\n");

    while (!gpsClient.getGPS())
    {
      delay(1000);
    }

    dataGPS = gpsClient.getCoordinates();
    Serial.println(dataGPS.latitude);
    Serial.println(dataGPS.longitude);
    

  }

  gpsClient.deactivateGPS();


  Serial.println("\n\n\n=============================  END GPS EXAMPLE =============================\r\n\n");



  // Serial.println("\n\n\n=============================  SMS EXAMPLE =============================\r\n\n");

  
  // smsClient.listSMS();

  // smsClient.readSMS(1);

  // smsClient.deleteSMS(0);
  
  // smsClient.deleteAllSMS();
  // smsClient.sendSMS("+57XXXXXXXXX","Hola desde el esp32");

  // Serial.println("\n\n\n=============================  END SMS EXAMPLE =============================\r\n\n");

  // Serial.println("\n\n\n=============================  SSL EXAMPLE =============================\r\n\n");

  // Serial.println("Los certificados son\n\n");
  // sslClient.showList();

  // sslClient.deleteCert("cert.pem");
  // sslClient.deleteCert("priv.pem");

  // const char* certName = "client_cert.pem";
  // const uint8_t* certData = (const uint8_t*)CERTIFICATE;
  // int certLen = strlen(CERTIFICATE);
  // sslClient.downloadCert(certName, certData, certLen);

  // sslClient.showList();

  // sslClient.serverVerify(certName);

  // Serial.println("\n\n\n=============================  END SSL EXAMPLE =============================\r\n\n");

  // Serial.println("\n\n\n=============================  MQTT EXAMPLE =============================\r\n\n");

  // mqttClient.startMqtt();
  // mqttClient.setBrokerUrl("broker.emqx.io");
  // mqttClient.setPort(1883);
  // mqttClient.setClient("esp-client",false);
  // mqttClient.setLastWillTopic("test/lastwill");
  // mqttClient.setLastWillMsg("I'm offline!");

  // if (mqttClient.connectMQTT())
  // {
  //   Serial.println("MQTT conectado correctamente!\r\n");

  //   mqttClient.subscribe("test/topic/juan29");

  //   delay(1000);

  //   mqttClient.publish("test/topic/juan28","{\"msg\":\"hello hades\"}");

  //   delay(1000);

  //   int count = 0;
  //   while (1)
  //   {
  //     mqttClient.readMsg();
  //     mqttClient.publish("test/topic/juan28","{\"msg\":\"hello hades\"}");
  //     if(count > 10){
  //       break;
  //     }
  //     count++;
  //     delay(5000);
  //   }

  // }
  // else
  // {
  //   Serial.println("Falló la conexión al broker MQTT.");
  // }

  // mqttClient.disconnect();

  // Serial.println("\n\n\n=============================  END MQTT EXAMPLE =============================\r\n\n");

  // Serial.println("\n\n\n=============================  HTTP EXAMPLE =============================\r\n\n");

  // httpclient.activate();

  // String ip = httpclient.getIP();
  // Serial.print("La ip es ");
  // Serial.println(ip);
  // Serial.print("\r\nEsperando 5 seg para ver la calidad de la señal \r\n");

  // delay(5000);

  // int data = httpclient.getSignalQuality();

  // Serial.print("The signal quality is  ");
  // Serial.println(data);
  // Serial.println("\r\n");

  // String url = "https://postman-echo.com/get?foo=hades&device=esp32";
  // if (httpclient.sendHttpGetTo(url))
  // {
  //   Serial.println("HTTP GET exitoso la repuesta es \r\n\n");
  //   Serial.println(httpclient.getResponse());
  // }
  // else
  // {
  //   Serial.println("HTTP GET falló");
  // }

  // Serial.print("\r\n\nRealizando POST\r\n\n");

  // url = "https://postman-echo.com/post";
  // String contentType = "application/json";
  // String body = "{\"mensaje\": \"Hola desde ESP32\"}";

  // if (httpclient.sendHttpPostTo(url, body, contentType))
  // {
  //   Serial.println("HTTP POST exitoso\r\n");
  // }
  // else
  // {
  //   Serial.println("HTTP POST falló");
  // }

  // Serial.println("\n\n\n=============================  END HTTP EXAMPLE =============================\r\n\n");
}

void loop()
{
  // put your main code here, to run repeatedly:
}
