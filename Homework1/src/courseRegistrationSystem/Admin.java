package courseRegistrationSystem;
import java.io.*;
import java.util.*;


public class Admin extends User implements AdminMethods, Serializable{

	
	
	
	public Admin(String username, String passport, String firstName, String lastName) {
		super(username,passport,firstName,lastName);
		
	}
	
	public Admin(String username, String passport) {
		super(username, passport);
	}


	public static void addCourse(Course c) {
		RegistrationSystem.courseList.add(c);
	}

	public static void create_course(String name, String ID) {
		Course c = new Course(name, ID);
		RegistrationSystem.courseList.add(c);
	}

	
	public static void delete_course(Course c) {
		for (int i = 0; i < RegistrationSystem.courseList.size(); i++) { // iterate over the courses 
			if (RegistrationSystem.courseList.get(i).getCourse_id().equals(c.getCourse_id())) {
				RegistrationSystem.courseList.remove(i);
			}
		}		
		for(Student s: RegistrationSystem.studentList) {
			if(s.registered(c)) {
				s.withdraw(c);
			}
		}
	}


	public static void edit_course(Course c) throws NumberFormatException, IOException {
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
		System.out.println("What do you want to edit for the course");
		System.out.println("Enter 1.Max Student Number 2.Course Instructor 3. Course Section number 4. Lacation");
		switch(Integer.parseInt(in.readLine())){
			case 1:
				System.out.println("Current max student is: " + c.getMax_student());
				System.out.println("Enter the max number you want to set");
				c.setMax_student(Integer.parseInt(in.readLine()));
				System.out.println("You have successful change the max number student");
				break;
			case 2:
				System.out.println("Current Instructor is: " + c.getCourse_instructor());
				System.out.println("Enter the Instrucot you want to set");
				c.setCourse_instructor(in.readLine());
				System.out.println("You have successful changed the instuctor");
				break;
			case 3:
				System.out.println("Current section number: " + c.getSection_num());
				System.out.println("Enter the section number you want to set");
				c.setSection_num(in.readLine());
				System.out.println("You have successful changed the section number");
				break;
			case 4:
				System.out.println("Current location: " + c.getLocation());
				System.out.println("Enter the location you want to set");
				c.setLocation(in.readLine());
				System.out.println("You have successful changed the location");
				break;
		}
		
	}


	public static void display_courseInf(Course c) {
		c.printInf();
		if(c.get_studentList().isEmpty()) {
			System.out.println("They are not students currently enrolled in class");
		}
		else {
			for(Student s: c.get_studentList()) {
				s.printInf();
			}
		}
		
	}


	public static void addStudent() throws IOException {
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
        System.out.println("Enter a first name:");
        String firstName = in.readLine();
        System.out.println("Enter a last name:");
        String lastName = in.readLine();
        System.out.println("Enter a username:");
        String username = in.readLine();
        System.out.println("Enter a password:");
        String password = in.readLine();
        System.out.println("Enter a id:");
        int id = Integer.parseInt(in.readLine());
        Student student = new Student(username, password, firstName, lastName,id);
        RegistrationSystem.studentList.add(student);
        System.out.println("You have successful add a new student");

	}

	public static void view_allCourses() {
		
		
		for (Course c : RegistrationSystem.courseList) {
			c.printInf();
            if(c.getCurrent_student()!=0) {
            	for(Student s: c.get_studentList()) {
            		s.printInf();
            	}
            }
            else {
            	System.out.println("They are no student in this course currently ");
            }
        }
	}

	public static void view_fullcourses() {
		for (Course c : RegistrationSystem.courseList) {// iterate over the courses 
			if(c.getCurrent_student()==0) {
				if(c.getCurrent_student()==c.getMax_student()) {
					c.printInf();
				}
			}
			else {
				System.out.print(" They are no full course currently ");
			}
		}
	}

	public static void write_fullList() throws FileNotFoundException {

		String fileName = "courselist.txt";
		
		try{
			FileWriter fileWriter = new FileWriter(fileName);
			BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);
			bufferedWriter.write("Course name, Course id");
			// iterate over the courses
			for (Course c : RegistrationSystem.courseList) {
				if(c.getCurrent_student()>=c.getMax_student()) {
					bufferedWriter.write(c.getCourse_name() + c.getCourse_id());	// write the course to the txxt for each one
					bufferedWriter.newLine();
				}
			}
			
			bufferedWriter.close();
		}

		catch (IOException exk) {
			System.out.println( "Error writing file '" + fileName + "'");
			exk.printStackTrace();
		}
	}
        

	public static void view_CourseStudentList(Course c) {
		System.out.println("Students in the course:");
		for(Student s: c.get_studentList()) { // iterate over the courses 
			System.out.print(s.getFirstname() + s.getLastname());
		}
		
	}

	public static void view_studentCourses(Student s) {
		System.out.println("Courses for the student:");
		s.view_myCourses();
	}

	public static void sort_courses() {
		  Collections.sort(RegistrationSystem.courseList);
          for(Course c : RegistrationSystem.courseList){
              c.printInf();
          }
	}
}
