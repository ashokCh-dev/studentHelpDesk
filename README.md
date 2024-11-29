The Student Help Desk Management System is a Java and C++ integrated application that provides an interface for students to report various issues and for administrators to manage and resolve them. The system leverages JNI (Java Native Interface) for seamless communication between Java and C++ components. The project includes the following features:

For Students: Report issues categorized as WiFi, hot water, water filter, room problems, or miscellaneous issues.
For Administrators: View reported issues, set resolution dates, and resolve issues based on their roles (DataCenter, PropertyManager, SAC).
Persistent data storage and retrieval for issue tracking across sessions.
Project Directory Structure
StudentHelpDesk/
│   ├── java/
│   │   ├── Main.java             # Java entry point
│   │   ├── build_and_run.sh         # Build and execution script
│   │   ├── projwithjni.cpp          # C++ logic implementation
│   │   ├── jni/
│   │   │   └── Main.h               # Files for persistent storage
│   │   ├── DataBaseWifi.txt
│   │   ├── DataBaseHotWater.txt
│   │   ├── DataBaseWaterFilter.txt
│   │   ├── DataBaseRoomProblem.txt
│   │   ├── DataBaseOther.txt
│   │   └── DataBaseResDate.txt
│   └── libprojwithjni.so            # Compiled native library
└── README.md                        # Project documentation

Dependencies
Java Development Kit (JDK)
GNU C++ Compiler (g++)
JNI Library

How to Run the Project

1.Clone the Repository:
git clone <https://github.com/ashokCh-dev/studentHelpDesk>
cd StudentHelpDesk

2.Compile the Code:
Navigate to the src directory.

To compile code in different OS

For Mac OS:-
Compile the Java code and generate the JNI headers:
javac -h . Main.java

Compile the C++ code and create the shared library:
g++ -std=c++11 -dynamiclib -o libprojwithjni.dylib -I/Library/Java/JavaVirtualMachines/jdk-21.jdk/Contents/Home/include -I/Library/Java/JavaVirtualMachines/jdk-21.jdk/Contents/Home/include/darwin projwithjni.cpp

4. Set Up Environment Variables
export DYLD_LIBRARY_PATH=/path/to/directory:$DYLD_LIBRARY_PATH

3.Run the Application:
java Main

For Linux:-
Compile the Java code and generate the JNI headers:
javac -h . Main.java

Compile the C++ code and create the shared library:
g++ -std=c++11 -shared -fPIC -o libprojwithjni.so \
    -I/usr/lib/jvm/java-21-openjdk-amd64/include \
    -I/usr/lib/jvm/java-21-openjdk-amd64/include/linux \
    projwithjni.cpp

4. Set Up Environment Variables
export LD_LIBRARY_PATH=/path/to/directory:$LD_LIBRARY_PATH

3.Run the Application:
java Main


Finally to end session:

4.End Session:
To end the file just press 3 and enter after logging out 
At the end of the session, data is automatically stored in the database files.

Note
Ensure you have the necessary permissions to execute scripts and the required environment variables (e.g., JAVA_HOME) set up.

