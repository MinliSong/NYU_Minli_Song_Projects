package courseRegistrationSystem;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Scanner;
import javafx.application.Application;
public class RegistrationSystem extends JavaFX implements Serializable {

	private static final long serialVersionUID = 1L;
	public static ArrayList<Course> courseList = new ArrayList<>(); 
	public static ArrayList<Student> studentList = new ArrayList<>();
	public static ArrayList<Admin> adminList = new ArrayList<>();

	private static Admin admin = new Admin("A", "A");
	private static Student student = new Student("S","S","First", "Second");

	
	public static void main(String[] args) throws NumberFormatException, IOException {

		File coursef = new File("courselist");
		File studentf = new File("studentlist");
		File adminf = new File("adminlist");
		
		if(coursef.exists()) {
			deserlizecourses();
		}
		else {
			inputCourses();
			adminList.add(admin);
			studentList.add(student);
		}
		//launch(args);
		
			if(studentf.exists()) {
				deserlizestudents();
			}
			if(adminf.exists()) {
				deserlizeadmins();
			}
			
			login();
			serializeData();
	}
	
	public static void inputCourses() { //Input Original Courses 
		try{
			java.io.File file = new java.io.File("MyUniversityCourses.csv"); //remove the subject line of the file
			Scanner input = new Scanner(file, "UTF-8");
			while(input.hasNext()) {
				String line = input.nextLine();
				String[] fields = line.split(",");
				String course_name = fields[0];
				String course_id = fields[1];
				int max_student = Integer.parseInt(fields[2]);
			    int current_student = Integer.parseInt(fields[3]);
			    ArrayList<Student> register_student_name = new ArrayList<>();
			    String course_instructor = fields[5];
			    String section_num = fields[6];
			    String location = fields[7];
			    Course c = new Course(course_name, course_id, max_student, current_student, register_student_name, course_instructor, section_num, location );
			    courseList.add(c);
			}
			input.close();
		}
		catch(FileNotFoundException e){
			System.out.println("File does not found");
		}
	}
	
	public static void login() throws NumberFormatException, IOException {
		String username, passport;
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
		System.out.println("Input 1 if you are a Student");
		System.out.println("Input 2 if you are a Admin");
		System.out.println("Input 3 Exit System");
		switch (Integer.parseInt(in.readLine())) {
			case 1: //Student Model
				System.out.println("Input your username!");
				username = in.readLine();
				System.out.println("Input your password!");
				passport = in.readLine();
				
				for(Student s: studentList) {
					if(s.getUsername().equals(username)&&s.getPassport().equals(passport)) {
						studentModel(s);
						break;
					}
					else {
						System.out.println("username and password are not correct! Please reenter ");
						login();
					}
				}
				break;
			case 2: //Admin Model
				System.out.println("Input your username!");
				username = in.readLine();
				System.out.println("Input your password!");
				passport = in.readLine();
				for(Admin a: adminList){
					if(a.getUsername().equals(username)&&a.getPassport().equals(passport)) {
						adminModel(a);
						break;
					}
					else {
						System.out.println("username and password are not correct! Please reenter ");
						login();
					}
				}
				break;
			case 3:
				break;
			default:
				System.out.println("Invaild imput, please login again");
				login();
		}
	}
	
	public static void adminModel(Admin a) throws NumberFormatException, IOException {
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
		System.out.println("Input 1 for Course Management");
		System.out.println("Input 2 for Reports");
		switch (Integer.parseInt(in.readLine())) {
		case 1: //Management Model
			adminManagementModel(a);
			break;
		case 2: //Report Model 
			adminReportModel(a);
			break;
		default:
			System.out.println("Invaild imput, please login again");
			login();
		}
	}

	private static void adminReportModel(Admin admin) throws NumberFormatException, IOException {
		System.out.println(courseList);
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
		String name, courseSection, firstname, lastname;
		System.out.println("This is Admin Course Report model ");
		System.out.println("1.View all courses ");
		System.out.println("2.View all courses that are FULL");
		System.out.println("3.Write to a file the list of course that are full");
		System.out.println("4.View the names of the students that are registered in a specific course ");
		System.out.println("5.View the list of courses that a given student is registered in  ");
		System.out.println("6.Sort the courses based on the current number of students registered ");
		System.out.println("7.Back to login page ");
		switch (Integer.parseInt(in.readLine())) {
			case 1: //View all courses
				Admin.view_allCourses();
				break;
			case 2: //View all courses that are FULL
				Admin.view_fullcourses();
				break;
			case 3: //Write to a file the list of course that are full
				Admin.write_fullList();
				break;
			case 4: //View the names of the students that are registered in a specific course
				System.out.println("Enter the name of the courses you want see");
				name = in.readLine();
				System.out.println("Enter the section of the courses you want see");
				courseSection = in.readLine();
				Course c = findCourse(name, courseSection);
				if(c!=null) {
					Admin.view_CourseStudentList(c);;
				}
				else {
					System.out.println("Can not find the course");
				}
				break;
			case 5: //View the list of courses that a given student is registered in
				System.out.println("Enter the first name of student you want see");
				firstname = in.readLine();
				System.out.println("Enter the last name of student you want see");
				lastname = in.readLine();
				Student s = findStudent(firstname,lastname);
				if(s!=null) {
					Admin.view_studentCourses(s);
				}
				else {
					System.out.println("I cannot find the student");
				}
				break;
			case 6: //Sort the courses based on the current number of students registered
				Admin.sort_courses();
				break;
			case 7: //Exit
				login();
				break;
			default:
				System.out.println("Please enter a vaild option");
				adminReportModel(admin);
		}
	}

	private static void adminManagementModel(Admin admin) throws NumberFormatException, IOException {
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
		String name, ID , courseSection;
		Course c; 
		System.out.println("This is Admin Course Management model ");
		System.out.println("1. Create a new course");
		System.out.println("2. Delete a new course");
		System.out.println("3. Edit a new course");
		System.out.println("4. Display information for a given course ");
		System.out.println("5. Register a student ");
		System.out.println("6. Back to login page  ");
		switch (Integer.parseInt(in.readLine())) {
			case 1: //Create a new course
				System.out.println("Enter the name of the courses you want create");
				name = in.readLine();
				System.out.println("Enter the ID of the courses you want create");
				ID = in.readLine();
				Admin.create_course(name, ID);
				break;
			case 2: //Delete a course
				System.out.println("Enter the name of the courses you want delete");
				name = in.readLine();
				System.out.println("Enter the section of the courses you want delete");
				courseSection = in.readLine();
				c = findCourse(name, courseSection);
				 if(c != null){
					 Admin.delete_course(c);
					 System.out.println("The " + c.getCourse_name()+ " has been deleted" );
				 }
				 else {
					 System.out.println("Cannot find the course");
				 }
				break;
			case 3: //Edit a course 
				System.out.println("Enter the name of the courses you want edit");
				name = in.readLine();
				System.out.println("Enter the section of the courses you want edit");
				courseSection = in.readLine();
				c = findCourse(name, courseSection);
				 if(c != null){
					 Admin.edit_course(c);
				 }
				 else {
					 System.out.println("Cannot find the course");
				 }
				break;
			case 4: //Display information for a given course (by course ID)
				System.out.println("Enter the id of the courses you want view");
				ID = in.readLine();
				c = findCourse(ID);
				 if(c != null){
					 Admin.display_courseInf(c);
				 }
				 else {
					 System.out.println("Cannot find the course");
				 }
	
				break;
			case 5: //Register a student
				Admin.addStudent();
				break;
			case 6: //Back to login page 
				login();
				break;
			default:
				System.out.println("Please enter a vaild option");
				adminManagementModel(admin);
		}
	}

	public static void studentModel(Student student) throws NumberFormatException, IOException {
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
		Course c;
        String courseName;
        String courseSection;
		System.out.println("This is Student Course Management model ");
		System.out.println("1. View all courses");
		System.out.println("2. View all courses that are not full");
		System.out.println("3. Register in a course ");
		System.out.println("4. Withdraw from a course");
		System.out.println("5. View all courses that the current student is registered in ");
		System.out.println("6. Back to login page ");
		switch (Integer.parseInt(in.readLine())) {
			case 1: //View all courses
				Student.view_allCourses();
				break;
			case 2: //View all courses that are not full
				Student.view_notFullCourses();
				break;
			case 3: //Register in a course
				System.out.print("Enter a course name");
                courseName = in.readLine();
                System.out.print("Enter course section");
                courseSection = in.readLine();
                c = findCourse(courseName,courseSection);
				if(c!=null) {
					if(!student.registered(c)) {
						student.register(c);
						c.register(student);
						System.out.println(String.format("You are now registered in  " + c.getCourse_name()));
					}
					else {
						 System.out.println("You're already registered on that course!" + c.getCourse_name());
						
					}
				}
				else {
					System.out.println("Cannot find the course");
					
				}
				break;
			case 4: //Withdraw from a course 
				if(student.get_myCourse().isEmpty()) {
					System.out.println("You are not registered in any course yet. ");
				}
				else{
					System.out.println("Enter a course name");
	                courseName = in.readLine();
	                System.out.println("Enter course section");
	                courseSection = in.readLine();
	                c = findCourse(courseName, courseSection);
					if(c!=null) {
						if(student.registered(c)) {
							student.withdraw(c);
							c.withdrawStudent(student);
							System.out.println(String.format("You are now withdraw from  " + c.getCourse_name()));
						}
						else {
							System.out.println("You were not registered on that course " + c.getCourse_name());
						}
					}
					else {
						System.out.println("Cannot find the course");
					}	
				}
				break;
			case 5: //View all courses that the current student is registered in 
				student.view_myCourses();
				break;
			case 6: //Back to login page 
				login();
				break;
			default:
				System.out.println("Please enter a vaild option");
				studentModel(student);
		}
	}
	

	public static Course findCourseByName(String name){ //find course by name 
	        for(Course c : courseList){
	            if(name.equals(c.getCourse_name())){
	                return c;
	            }
	        }
	        return null;
	    }

    public static Course findCourse(String name, String sectionNumber){ //find course by name and section 
        for(Course c : courseList){
            if(name.equals(c.getCourse_name()) && sectionNumber.equals(c.getSection_num())){
                return c;
            }
        }
        return null;
    }

    public static Course findCourse(String id){ // find course by id 
        for(Course c : courseList){
            if(id.equals(c.getCourse_id())){
                return c;
            }
        }
        return null;
    }

    public static Student findStudent(String firstName, String lastName){ //find student
        for(Student s : studentList){
            if(firstName.equals(s.getFirstname()) && lastName.equals(s.getLastname())){
                return s;
            }
        }
        return null;
    }
    
	public static void serializeData(){
        try{
            
            FileOutputStream coursefo= new FileOutputStream("courselist");
            ObjectOutputStream courseoos= new ObjectOutputStream(coursefo);
            courseoos.writeObject(courseList);
            courseoos.close();
            coursefo.close();

            FileOutputStream studentfo = new FileOutputStream("studentlist");
            ObjectOutputStream studentoos = new ObjectOutputStream(studentfo);
            studentoos.writeObject(studentList);
            studentoos.close();
            studentfo.close();
            
            FileOutputStream adminfo = new FileOutputStream("adminlist");
            ObjectOutputStream adminoos = new ObjectOutputStream(adminfo);
            adminoos.writeObject(adminList);
            adminoos.close();
            adminfo.close();
        }catch(IOException ioe){
            ioe.printStackTrace();
        }
    }
	
	public static void deserlizecourses() {
	     
	     try {
	    	 
	     FileInputStream coursefi = new FileInputStream("courselist");
	     ObjectInputStream courseois = new ObjectInputStream(coursefi);
	     courseList = (ArrayList<Course>)courseois.readObject();
	     courseois.close();
	     coursefi.close();
	          
	     }
	     catch(IOException ioe) {
		     ioe.printStackTrace();
		     return;
		 }
	     catch(ClassNotFoundException cnfe) {
	    	 cnfe.printStackTrace();
		     return;
	     }
	     
	}
	
	public static void deserlizestudents() {
		
		 try {
			 FileInputStream studentfi = new FileInputStream("studentlist");
		     ObjectInputStream studentois = new ObjectInputStream(studentfi);
		     studentList = (ArrayList<Student>)studentois.readObject();
		     studentois.close();
		     studentfi.close();
		     
		 }
		 catch(IOException ioe) {
		     ioe.printStackTrace();
		     return;
		 }
	     catch(ClassNotFoundException cnfe) {
	    	 cnfe.printStackTrace();
		     return;
	     }
	}
	
	public static void deserlizeadmins() {
	     
	     try {    
	    	 
		     FileInputStream adminfi = new FileInputStream("adminlist");
		     ObjectInputStream adminois = new ObjectInputStream(adminfi);
		     adminList = (ArrayList<Admin>)adminois.readObject();
		     adminois.close();
		     adminfi.close();
	          
	     }
	     catch(IOException ioe) {
		     ioe.printStackTrace();
		     return;
		 }
	     catch(ClassNotFoundException cnfe) {
	    	 cnfe.printStackTrace();
		     return;
	     }
	    
	     
	}
}
