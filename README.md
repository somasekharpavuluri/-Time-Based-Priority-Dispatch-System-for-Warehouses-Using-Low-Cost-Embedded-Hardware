# 🏭 Time-Based Priority Dispatch System for Warehouses
![image](https://github.com/user-attachments/assets/5c839cf7-6943-43f6-af49-2e3e0618354a)

## 📌 Overview

A **real-time warehouse automation system** using **ESP32, RFID, Arduino Nano, RTC, and Google Firebase**, designed for **small to medium-scale warehouses**. It enables **priority-based dispatch** using smart classification and tracking zones, reducing human error and improving operational efficiency.

---

## 🚀 Key Features

- 📦 **Real-Time Product Tracking** using RFID and Firebase
- ⏳ **Priority Classification** (High / Medium / Low) based on delivery urgency
- 📍 **Automated Zone & Block Allocation**
- 🌐 **Firebase Integration** via ESP32 Wi-Fi
- 🕒 **RTC Module** for scheduled dispatch
- 📊 **Live Dashboard** built with React.js
- 📱 **Mobile App** (MIT App Inventor) for vehicle control

---

## 🧠 Warehouse Operation Logic
![image](https://github.com/user-attachments/assets/72fd03b4-8c6b-4444-aff0-aa8e4830cc30)
![image](https://github.com/user-attachments/assets/61939a33-0a0e-4aa5-acb3-0ffbf9bb3ef0)

1. Products scanned via RFID upon arrival.
2. Delivery deadline is evaluated.
3. Based on urgency, products are placed into:
   - **Zone 3**: High Priority (≤ 24 hrs)
   - **Zone 2**: Medium Priority (≤ 3 days)
   - **Zone 1**: Low Priority (> 3 days)
4. Zone-wise storage is divided into **Blocks A, B, C**.
5. RTC tracks time until dispatch.
6. ESP32 sends live updates to Firebase.
7. Web dashboard displays status and logs all actions.
8. Final RFID scan marks product as **"Dispatched"**.

---

## 🧰 Hardware Used

### 🔌 Microcontroller & Communication
- **Arduino Nano** – Controls vehicle functions
- **HC-05 Bluetooth Module** – Mobile interface for manual movement

### ⚙️ Control & Motion
- **L298N Motor Driver**
- **4 × DC Gear Motors** on a 4-wheel chassis
- **Breadboard & Jumper Wires**

### 📦 Tracking & Automation
- **RC522 RFID Reader**
- **Passive RFID Tags** – Product-level ID
- **RTC Module** – Accurate dispatch timing

### 📶 Connectivity & Cloud
- **ESP32 Wi-Fi Module** – Pushes data to **Firebase Realtime DB**

### ⚡ Power Supply
- **12V Adapter** + 5V Regulator Circuit

---

## 📱 Mobile App
![image](https://github.com/user-attachments/assets/49d01409-3eb9-441a-a3d1-9718bad58ac3)

Built using **MIT App Inventor**, the Android app controls the warehouse vehicle over Bluetooth.

### Controls:
- **F** – Forward
- **B** – Backward
- **L** – Left
- **R** – Right
- **S** – Stop

Used for manual navigation of product vehicles within the warehouse.

---

## 💻 Software Stack

| Tool           | Purpose                        |
|----------------|-------------------------------|
| Arduino IDE    | Code development (ESP32, Nano) |
| Firebase       | Real-time database & sync      |
| React.js       | Frontend dashboard             |
| Node.js        | Backend (optional)             |
| Firebase Hosting | Dashboard deployment         |
| MIT App Inventor | Android app for control      |

---

## 🖥️ Web Dashboard
![image](https://github.com/user-attachments/assets/2e210064-c981-4efe-8647-5ab95c5e22ec)

Live warehouse view:
- 🗂️ Zone & Block Allocation Overview
- ⌛ Countdown to dispatch
- 📋 Product details: name, ID, zone, customer, deadline
- 🧾 Dispatch history and current status

---

## 🔁 Dispatch Logic

```text
1. RFID Tag Detected → ESP32 Reads Data
2. Priority Evaluated → Zone Assigned
3. Firebase Updated → Dashboard Reflects Change
4. RTC Tracks Time Until Dispatch
5. Final Scan → Mark as Dispatched
🔮 Future Enhancements
🔧 Full automation with line-following robots or AGVs

🤖 AI-based path optimization and load balancing

📲 Mobile + Firebase UI integration for remote access

☁️ Offline data caching with background sync

📚 Use Cases
Educational warehouse automation projects

Real-time smart logistics in small enterprises

Low-cost automation for MSME dispatch systems

