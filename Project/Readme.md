# **Crypto S/W App \- Complete Setup and Build Guide for Windows**

This guide provides definitive, step-by-step instructions to set up the environment, build the required libraries, and run the Qt-based cryptography application. It incorporates solutions to common build issues.

## **1\. Overview**

This project is a C++ desktop application for Windows that performs fundamental cryptographic operations using the Crypto++ library for the backend and the Qt framework for the graphical user interface.

### **Features**

* **RSA:** Key-pair generation, encryption, and decryption of files.  
* **DSA:** Key-pair generation, digital signature generation for files, and signature verification.  
* **GUI:** A simple, tab-based interface to access all cryptographic functions.  
* **Configuration:** Cryptographic parameters (like key sizes) are managed in config.ini.

## **2\. Tech Stack**

* **Language:** C++17  
* **GUI Framework:** Qt 6 (with MinGW 64-bit compiler)  
* **Cryptography Library:** Crypto++ 8.x

## **Part 1: Environment Setup**

This part prepares your computer with all the necessary tools.

### **Step 1.1: Install Qt & Qt Creator**

1. **Download:** Get the Qt Online Installer from the [official Qt website](https://www.google.com/search?q=https://www.qt.io/download-qt-installer-oss).  
2. **Account:** You will need a free Qt Account to run the installer.  
3. **Component Selection (Crucial):** On the "Select Components" screen:  
   * Under the latest Qt version (e.g., Qt 6.7.2), expand the tree and check the box for **MinGW**. This is your 64-bit C++ compiler.  
   * Under "Tools", ensure **Qt Creator** and **CMake** are also checked.

### **Step 1.2: Install Git for Windows (to get helper tools)**

The Crypto++ build script requires standard command-line tools like grep and cut which are not included with Windows. The easiest way to get them is by installing Git.

1. **Download:** Go to the [Git for Windows website](https://git-scm.com/download/win) and run the installer.  
2. **Installer Settings (Crucial):**  
   * Proceed through the installer with the default options until you reach the **"Adjusting your PATH environment"** screen.  
   * Select the recommended option: **"Git from the command line and also from 3rd-party software"**. This makes grep and other tools available in all command prompts.  
3. Complete the installation with the remaining default options.

## **Part 2: Build the Crypto++ Library**

This section details the definitive method for compiling libcryptopp.a to avoid all common errors.

### **Step 2.1: Download and Extract Crypto++**

1. **Download:** Get the latest Crypto++ source code as a ZIP file from the [official website](https://www.google.com/search?q=https://www.cryptopp.com/%23download).  
2. **Extract:** Unzip the file to a simple path, for example, C:\\dev\\cryptopp890.

### **Step 2.2: Compile the Library (The Final, Correct Command)**

1. **Open the Correct Command Prompt:** Go to your Windows Start Menu and open the **"Qt 6.x.x (MinGW ... 64-bit)"** command prompt.  
2. **Navigate to Directory:** In the prompt, navigate to your Crypto++ source folder:  
   cd C:\\dev\\cryptopp890

3. **Clean Up (Important):** If you have any old build files, delete them.  
4. **Run the Build Command:** Execute the following command exactly as written. This command tells the build script which compiler and flags to use, preventing all errors.  
   mingw32-make CXX=g++ CXXFLAGS="-DNDEBUG \-g \-O2 \-m64" static

5. **Verify:** After a few minutes, the process will complete. Check your C:\\dev\\cryptopp890 folder. You will now have a correctly built, 64-bit libcryptopp.a file.

## **Part 3: Configure and Run the Qt Project**

### **Step 3.1: Arrange Project Files**

1. Create a project folder (e.g., C:\\Projects\\CryptoApp).  
2. Place all the C++ source files (.cpp, .h), the UI file (.ui), the project file (.pro), and the config file (.ini) into this folder.  
3. **Copy the Library:** Copy your newly built libcryptopp.a into this folder.  
4. **Copy the Headers:**  
   * Create a new subfolder inside your project named cryptopp\_headers.  
   * Copy all the header files (.h files) from the Crypto++ source (C:\\dev\\cryptopp890) into this cryptopp\_headers folder.

Your final project structure must look like this:

C:\\Projects\\CryptoApp\\  
│  
├── CryptoApp.pro  
├── config.ini  
├── cryptomanager.cpp  
├── cryptomanager.h  
├── libcryptopp.a          \<-- The compiled 64-bit library  
├── main.cpp  
├── mainwindow.cpp  
├── mainwindow.h  
├── mainwindow.ui  
│  
└─── cryptopp\_headers/     \<-- Folder with all .h files from Crypto++  
     ├── aes.h  
     ├── dsa.h  
     └── ... (and all other headers)

### **Step 3.2: Build and Run in Qt Creator**

1. **Open Project:** Start Qt Creator, go to **File \> Open File or Project...**, and select your CryptoApp.pro file.  
2. **Configure Kit:** The "Configure Project" dialog should appear. Select the Desktop Qt MinGW 64-bit kit and click "Configure Project".  
3. **Clean Project:** To be safe, first clean the project via the menu: **Build \> Clean Project "CryptoApp"**.  
4. **Build Project:** Click the **Build** icon (a hammer) in the bottom-left panel.  
5. **Run Application:** Click the **Run** icon (a green play button).

The application should now launch and be ready to use.

## **4\. How to Use**

1. **Generate Keys:** Click the "Generate Key Pair" buttons on both the RSA and DSA tabs first. This creates the necessary .key files in your build directory.  
2. **Encrypt/Sign:** Use the "Upload" buttons to select a file, then click the "Process" button.  
3. **Save Output:** A "Save As..." dialog will appear for you to save the encrypted file, decrypted file, or signature.  
4. **Verify:** To verify a signature, you must upload the original file and its corresponding signature file.