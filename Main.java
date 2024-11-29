import java.util.Scanner;

public class Main {
    // Load the native library for JNI (Java Native Interface)
    static {
        System.loadLibrary("projwithjni");
    }

    // Declare native methods that interact with the JNI
    private native void processStudentQuery(String rollno, String problem, int choice, String ch);
    private native void adminViewQuery(String adminType, String ch);
    private native void adminResolveDate(String rollno, String roomno, String resdate, String adminType, String choice, String ch);
    private native void endSessionQuery();
    private native void startSessionQuery();

    public static void main(String[] args) {
        // Create Scanner object to read user input
        Scanner scanner = new Scanner(System.in);
        // Create an instance of the Main class
        Main main = new Main();
        // Start a session
        main.startSessionQuery();
        
        // Main loop for user interaction
        while (true) {
            System.out.println("Welcome to the Student Help Desk!");
            System.out.println("Select User Type: ");
            System.out.println("1. Student");
            System.out.println("2. Admin");
            System.out.println("3. End Session");
            System.out.print("Enter your choice: ");
            
            // Read user choice for the type of user (Student, Admin, End Session)
            String userType = scanner.nextLine();
            
            // If user is a Student
            if (userType.equals("Student") || userType.equals("1")) {
                System.out.print("Enter your roll number: ");
                String rollno = scanner.nextLine();
                System.out.println("Select your problem:");
                
                // List of issues available for students
                System.out.println("1. WiFi Issue");
                System.out.println("2. Hot Water Issue");
                System.out.println("3. Water Filter Issue");
                System.out.println("4. Room Problems");
                System.out.println("5. Other");
                System.out.print("Enter your choice: ");
                
                // Read student's choice of problem
                String choice = scanner.nextLine();
                
                // Handle different types of issues based on student's choice
                switch (choice) {
                    case "WiFi Issue":
                    case "1":
                        // Handle WiFi-related issues
                        System.out.println("Select the type of your problem: ");
                        System.out.println("1. Router not working");
                        System.out.println("2. Wifi speed slow");
                        System.out.print("Enter your choice: ");
                        String ch = scanner.nextLine();
                        System.out.println("Enter your room no: ");
                        String roomno = scanner.nextLine();
                        main.processStudentQuery(rollno, roomno, 1, ch); // Call JNI method
                        break;
                    case "Hot Water Issue":
                    case "2":
                        // Handle Hot Water-related issues
                        System.out.println("Select the type of your problem: ");
                        System.out.println("1. Not getting hot water during respective timing");
                        System.out.println("2. Geyser not working");
                        System.out.print("Enter your choice: ");
                        ch = scanner.nextLine();
                        System.out.println("Enter the nearest room number to the washroom: ");
                        roomno = scanner.nextLine();
                        main.processStudentQuery(rollno, roomno, 2, ch); // Call JNI method
                        break;
                    case "Water Filter Issue":
                    case "3":
                        // Handle Water Filter-related issues
                        System.out.println("Enter the nearest room number to the water filter: ");
                        roomno = scanner.nextLine();
                        main.processStudentQuery(rollno, roomno, 3, ""); // Call JNI method
                        break;
                    case "Room Problems":
                    case "4":
                        // Handle Room-related problems
                        System.out.println("Enter your room no: ");
                        roomno = scanner.nextLine();
                        System.out.println("Select the type of your problem: ");
                        System.out.println("1. Switchboard not working");
                        System.out.println("2. Lightbulb not working");
                        System.out.println("3. Broken Furniture");
                        System.out.println("4. Water leakage");
                        System.out.println("5. Other");
                        System.out.print("Enter your choice: ");
                        ch = scanner.nextLine();
                        main.processStudentQuery(rollno, roomno, 4, ch); // Call JNI method
                        break;
                    case "Other":
                    case "5":
                        // Handle other issues
                        System.out.println("Enter your room no: ");
                        roomno = scanner.nextLine();
                        main.processStudentQuery(rollno, roomno, 5, ""); // Call JNI method
                        break;
                    default:
                        // Handle invalid choice
                        System.out.println("Invalid choice.");
                        return;
                }
            } 
            // If user is an Admin
            else if (userType.equals("Admin") || userType.equals("2")) {
                System.out.println("Select admin type");
                System.out.println("1. DataCenter");
                System.out.println("2. PropertyManager");
                System.out.println("3. SAC");
                String adminType = scanner.nextLine();
                System.out.println("Enter your admin password: ");
                String password = scanner.nextLine();
                
                // Check if the admin password is correct
                if (password.equals("123")) {
                    System.out.println("Admin login successful.");
                    
                    // Handle different admin types
                    if (adminType.equals("DataCenter") || adminType.equals("1")) {
                        while (true) {
                            // Admin menu options for DataCenter
                            System.out.println("1. Get queries");
                            System.out.println("2. Set resolved date");
                            System.out.println("3. Resolve");
                            System.out.println("4. LogOut");
                            System.out.print("Enter your choice: ");
                            int choice = scanner.nextInt();
                            scanner.nextLine();
                            
                            // Handle DataCenter-specific admin actions
                            if (choice == 1) {
                                main.adminViewQuery(adminType, ""); // Call JNI method to get queries
                            } else if (choice == 2) {
                                System.out.println("Enter the roll number of the student: ");
                                String rollno = scanner.nextLine();
                                System.out.println("Enter the resolved date (DD-MM-YYYY): ");
                                String resolvedDate = scanner.nextLine();
                                main.adminResolveDate(rollno, "", resolvedDate, adminType, "wifi", ""); // Call JNI method to resolve
                            } else if (choice == 3) {
                                System.out.println("Enter the roll number of the student: ");
                                String rollno = scanner.nextLine();
                                main.adminResolveDate(rollno, "", "", adminType, "wifi", ""); // Call JNI method to resolve
                            } else if (choice == 4) {
                                System.out.println("Session Ended");
                                break;
                            } else
                                System.out.println("Invalid choice.");
                        }
                    } 
                    // Similar blocks for PropertyManager and SAC admins
                    else if (adminType.equals("PropertyManager") || adminType.equals("2")) {
                        while (true) {
                            // Admin menu options for PropertyManager
                            System.out.println("Enter your choice");
                            System.out.println("1. Get queries");
                            System.out.println("2. Set resolved date");
                            System.out.println("3. Resolve");
                            System.out.println("4. LogOut");
                            int choice = scanner.nextInt();
                            scanner.nextLine();
                            String ch = "";
                            String ch1 = "";
                            
                            if (choice != 4) {
                                System.out.println("Select the Type of queries: ");
                                System.out.println("1. Hot Water");
                                System.out.println("2. Water Filter");
                                System.out.println("3. Room Problem");
                                System.out.println("4. Other");
                                System.out.print("Enter your choice: ");
                                ch = scanner.nextLine();
                            }
                            
                            // Handle different query types and actions
                            if (ch.equals("1"))
                                ch = "hotwater";
                            else if (ch.equals("2"))
                                ch = "waterfilter";
                            else if (ch.equals("3"))
                                ch = "roomproblem";
                            else if (ch.equals("4")) {
                                ch1 = scanner.nextLine();
                                ch = "other";
                            } else {
                                ch = "";
                                System.out.println("Invalid choice");
                            }
                            // Handle property manager-specific actions
                            if (!ch.equals("")) {
                                if (choice == 1) {
                                    main.adminViewQuery(adminType, ch); // Call JNI method to view queries
                                } else if (choice == 2) {
                                    System.out.println("Enter the roll number of the student: ");
                                    String rollno = scanner.nextLine();
                                    System.out.println("Enter the room number: ");
                                    String roomno = scanner.nextLine();
                                    System.out.println("Enter the resolved date (DD-MM-YYYY): ");
                                    String resolvedDate = scanner.nextLine();
                                    main.adminResolveDate(rollno, roomno, resolvedDate, adminType, ch, ch1); // Call JNI method to resolve
                                } else if (choice == 3) {
                                    System.out.println("Enter the roll number of the student: ");
                                    String rollno = scanner.nextLine();
                                    System.out.println("Enter the room number: ");
                                    String roomno = scanner.nextLine();
                                    main.adminResolveDate(rollno, roomno, "", adminType, ch, ch1); // Call JNI method to resolve
                                } else if (choice == 4) {
                                    System.out.println("Session Ended");
                                    break;
                                } else
                                    System.out.println("Invalid choice.");
                            }
                        }
                    } 
                    // Handle SAC admin actions
                    else if (adminType.equals("SAC") || adminType.equals("3")) {
                        while (true) {
                            // Admin menu options for SAC
                            System.out.println("Enter your choice");
                            System.out.println("1. Get queries");
                            System.out.println("2. Set resolved date");
                            System.out.println("3. Resolve");
                            System.out.println("4. LogOut");
                            int choice = scanner.nextInt();
                            scanner.nextLine();
                            if (choice == 1)
                                main.adminViewQuery(adminType, ""); // Call JNI method to view queries
                            else if (choice == 2) {
                                System.out.println("Enter the roll number of the student: ");
                                String rollno = scanner.nextLine();
                                System.out.println("Enter the resolved date (DD-MM-YYYY): ");
                                String resolvedDate = scanner.nextLine();
                                main.adminResolveDate(rollno, "", resolvedDate, adminType, "other", ""); // Call JNI method to resolve
                            } else if (choice == 3) {
                                System.out.println("Enter the roll number of the student: ");
                                String rollno = scanner.nextLine();
                                main.adminResolveDate(rollno, "", "", adminType, "other", ""); // Call JNI method to resolve
                            }
                            else if (choice == 4) {
                                System.out.println("Session Ended");
                                break;
                            } 
                            else
                                System.out.println("Invalid choice.");
                        }
                    }
                } 
                else
                    System.out.println("Incorrect password");
            } 
            // If user wants to end the session
            else if (userType.equals("3")) {
                System.out.println("Session Ended");
                main.endSessionQuery(); // Call JNI method to end the session
                break;
            } else
                // Handle invalid input
                System.out.println("Invalid choice.");
        }
    }
}
