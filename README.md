<div align="center">

  <h1>📄 Advanced Automated Collating and Stapling Mechanism</h1>
  <h3>Mechatronics & Office Automation Prototype</h3>

  <p>
    An electromechanical system designed to streamline document preparation by <br />
    automating the sorting, aligning, and stapling of multi-page documents.
  </p>

  <p>
    <img src="https://img.shields.io/badge/Arduino_Mega-00979D?style=for-the-badge&logo=Arduino&logoColor=white" alt="Arduino Mega" />
    <img src="https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++" />
    <img src="https://img.shields.io/badge/Automation-orange?style=for-the-badge&logo=autodesk&logoColor=white" alt="Automation" />
    <img src="https://img.shields.io/badge/3D_Printing-red?style=for-the-badge&logo=bambulab&logoColor=white" alt="3D Printing" />
  </p>

  <img src="assets/final.jpg" alt="Automated Collator Machine" width="100%" height="300px">

</div>

<br />

## 📌 Overview

The **Advanced Automated Collating and Stapling Mechanism** was developed to address the time-consuming and repetitive task of manual document collation in high-volume office environments, specifically targeted for the General Services Office (GSO) of UST-Legazpi.

This prototype automates the entire workflow: dispensing individual sheets from multiple bins, transporting them via conveyor, aligning the stack, and stapling the final set. By integrating sensor-based feedback and robust motor control, the system significantly reduces human error and processing time.

---

## 🎯 Key Features

| Feature | Description |
| :--- | :--- |
| **📑 Multi-Bin Dispensing** | Three separate input bins capable of dispensing sheets sequentially to form a complete document set. |
| **⚙️ Conveyor Transport** | A belt-driven mechanism powered by DC gear motors that transports papers smoothly from bins to the staging area. |
| **📏 Auto-Alignment** | Mechanical arms that automatically align the loose papers into a neat stack before stapling. |
| **📎 Automated Stapling** | A high-torque servo mechanism that triggers a stapler once the document set is verified as complete. |
| **🚨 Error Detection** | Integrated IR and Ultrasonic sensors to detect paper jams, empty bins, or misalignment in real-time. |
| **📟 LCD Status Monitor** | A 20x4 LCD screen that provides live feedback on the current operation status (e.g., "Collating", "Stapling", "Error"). |

---

## 🧠 System Architecture

The system is controlled by an **Arduino Mega 2560**, which handles the logic for synchronizing multiple motors and reading sensor data.

### 🛠️ Hardware Components

| Component | Function |
| :--- | :--- |
| **Arduino Mega 2560** | The main microcontroller unit (MCU) managing all IOs. |
| **IR Sensors (E18-D80NK)** | Detects the passage of paper and obstacles to ensure correct dispensing. |
| **Ultrasonic Sensors (HC-SR04)** | Monitors the stack height and presence of paper in bins. |
| **DC Gear Motors** | Provides high torque for the conveyor belt and paper rollers. |
| **L298N Motor Drivers** | Controls the direction and speed of the DC motors. |
| **MG996R Servo Motors** | Actuates the alignment arms and the stapling mechanism. |
| **LCD 20x4 Display** | User interface for system status and troubleshooting. |
| **Power Supply (12V 30A)** | Provides stable power for the motors and microcontroller. |

---

## 📸 Project Gallery

<table>
  <tr>
    <td width="50%">
      <h3 align="center">Internal Mechanism</h3>
      <img src="assets/collating-machine.jpg" alt="Internal Mechanism" width="100%">
    </td>
    <td width="50%">
      <h3 align="center">Circuitry & Logic</h3>
      <img src="assets/microprocessor.jpeg" alt="Arduino Mega Logic" width="100%">
    </td>
  </tr>
  <tr>
    <td width="50%">
      <h3 align="center">LCD Interface</h3>
      <img src="assets/lcd.jpeg" alt="LCD Display" width="100%">
    </td>
    <td width="50%">
      <h3 align="center">Motor Assembly</h3>
      <img src="assets/gear-motors.jpg" alt="Gear Motors" width="100%">
    </td>
  </tr>
</table>

---

## 💻 Software & Tools Used

* **IDE:** Arduino IDE (C++)
* **3D Modeling:** SketchUp (for chassis and printed parts)
* **Slicing:** Ultimaker Cura
* **Circuit Design:** Fritzing

---

## 👥 Authors

**Lance Madel S. Esureña** & **Mar Joefrey M. Calleja**
<br>
*Bachelor of Science in Computer Engineering*
<br>
*University of Santo Tomas - Legazpi*

---

<div align="center">
  <p>Developed by <b>Lance Esureña</b></p>
  <p>
    <a href="https://www.linkedin.com/in/lance-madel-esure%C3%B1a-ba4871282/">
      <img src="https://img.shields.io/badge/LinkedIn-Connect-blue?style=flat-square&logo=linkedin" alt="LinkedIn">
    </a>
    <a href="https://lance-portfolio-v1.vercel.app/">
      <img src="https://img.shields.io/badge/Portfolio-Visit-orange?style=flat-square&logo=vercel" alt="Portfolio">
    </a>
  </p>
</div>
