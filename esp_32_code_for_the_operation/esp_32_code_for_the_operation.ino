#include <WiFi.h>
#include <FirebaseESP32.h>
#include <SPI.h>
#include <MFRC522.h>

// Wi-Fi and Firebase configuration
#define WIFI_SSID "Karthik7"
#define WIFI_PASSWORD "12345678"
#define FIREBASE_HOST "https://rfidweb-4ab93-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "4DsFdmh1CPbyNZ2QmbRVEw2aj7F4yY6OW5xFOgjf"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// RFID setup
#define SS_PIN 21
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);

// LED pin
const int ledPin = 2;

// Zone Tag IDs
String zone1Tag = "97DCD9CA";
String zone2Tag = "8A43CBFF";
String zone3Tag = "FA4C03FF";

// Product Tag IDs
String product1Tag = "437FCD97"; // Mobile
String product2Tag = "5AE9CFFF"; // Laptop

// Product structure
struct Product {
  String tag;
  String name;
  String customer;
  String address;
  String arrivalDate;
  String deliveryDate;
};

// Product database
Product products[] = {
  {"437FCD97", "Mobile", "Karthik", "Vijayawada", "23-05-2025", "25-05-2025"},
  {"5AE9CFFF", "Laptop", "Somasekhar", "Guntur", "23-05-2025", "24-05-2025"}
};

bool productScanned = false;
bool zoneReached = false;
bool dispatched = false;

Product currentProduct;
String requiredZone = "";
String requiredZoneTag = "";

// Convert dd-mm-yyyy to approx days
long dateToDays(String dateStr) {
  int day = dateStr.substring(0, 2).toInt();
  int month = dateStr.substring(3, 5).toInt();
  int year = dateStr.substring(6, 10).toInt();
  return year * 365L + month * 30L + day;
}

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n✅ Wi-Fi Connected");

  // Setup Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("Place product RFID tag to begin...");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;

  String tagID = getTagID();
  Serial.println("Tag Detected: " + tagID);

  // PRODUCT SCAN
  if (!productScanned) {
    bool found = false;
    for (int i = 0; i < sizeof(products) / sizeof(products[0]); i++) {
      if (tagID == products[i].tag) {
        currentProduct = products[i];
        found = true;
        break;
      }
    }

    if (found) {
      Serial.println("Product Identified!");
      Serial.println("Product: " + currentProduct.name);
      Serial.println("Customer: " + currentProduct.customer);
      Serial.println("Address: " + currentProduct.address);
      Serial.println("Arrived: " + currentProduct.arrivalDate);
      Serial.println("Delivery Date: " + currentProduct.deliveryDate);

      long arrivedDays = dateToDays(currentProduct.arrivalDate);
      long deliveryDays = dateToDays(currentProduct.deliveryDate);
      long diff = deliveryDays - arrivedDays;

      if (diff == 1) {
        requiredZone = "Zone 3";
        requiredZoneTag = zone3Tag;
      } else if (diff == 2) {
        requiredZone = "Zone 2";
        requiredZoneTag = zone2Tag;
      } else if (diff == 3) {
        requiredZone = "Zone 1";
        requiredZoneTag = zone1Tag;
      } else {
        requiredZone = "Unknown Zone";
        requiredZoneTag = "";
      }

      Serial.println("→ Move this to " + requiredZone);
      sendToFirebase("Identified");
      productScanned = true;
      Serial.println("Now scan the ZONE tag...");
    } else {
      Serial.println("❌ Unknown product tag!");
    }
  }

  // ZONE SCAN
  else {
    if (tagID == requiredZoneTag) {
      if (!zoneReached) {
        Serial.println("✅ " + currentProduct.name + " reached " + requiredZone);
        digitalWrite(ledPin, HIGH);
        sendToFirebase("In Zone");
        zoneReached = true;
      } else if (!dispatched) {
        Serial.println("🚚 " + currentProduct.name + " dispatched from " + requiredZone);
        digitalWrite(ledPin, LOW);
        sendToFirebase("Dispatched");

        // Reset
        productScanned = false;
        zoneReached = false;
        dispatched = false;
        requiredZone = "";
        requiredZoneTag = "";

        Serial.println("\n🔄 Ready for next product...\n");
        delay(2000);
        Serial.println("Place next product RFID tag to begin...");
      }
    } else {
      Serial.println("❌ Wrong Zone! Expected: " + requiredZone + ", but scanned: " + tagID);
      digitalWrite(ledPin, LOW);
    }
  }

  delay(1500);
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

// Convert tag bytes to hex string
String getTagID() {
  String tagID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    String byteHex = String(mfrc522.uid.uidByte[i], HEX);
    if (byteHex.length() < 2) tagID += "0";
    tagID += byteHex;
  }
  tagID.toUpperCase();
  return tagID;
}

// Send data to Firebase
void sendToFirebase(String status) {
  String path = "/products/" + currentProduct.tag;

  Firebase.setString(fbdo, path + "/product", currentProduct.name);
  Firebase.setString(fbdo, path + "/customer", currentProduct.customer);
  Firebase.setString(fbdo, path + "/address", currentProduct.address);
  Firebase.setString(fbdo, path + "/arrivalDate", currentProduct.arrivalDate);
  Firebase.setString(fbdo, path + "/deliveryDate", currentProduct.deliveryDate);
  Firebase.setString(fbdo, path + "/zone", requiredZone);
  Firebase.setString(fbdo, path + "/status", status);
  Firebase.setInt(fbdo, path + "/timestamp", millis());
}