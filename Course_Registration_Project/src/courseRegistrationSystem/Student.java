package courseRegistrationSystem;

import java.io.Serializable;
import java.util.ArrayList;

public class Student extends User implements StudentMethods, Serializable{
	
	
	private ArrayList<Course> courses = new ArrayList<>();
	private int id;
	
	public Student(String username, String passport, String firstName, String lastName) {
		super(username,passport,firstName,lastName);

	}
	public Student(String username, String passport, String firstName, String lastName, int id) {
		super(username,passport,firstName,lastName);
		this.id = id;
	}
	
	public static void view_allCourses() {
		 for (Course c : RegistrationSystem.courseList) { // iterate over the courses 
             c.printInf();
         }
	}


	public static void view_notFullCourses() {
		 for (Course c : RegistrationSystem.courseList) { // iterate over the courses 
             if (c.getCurrent_student() < c.getMax_student()) {
                c.printInf();
             }
         }
	}

	@Override
	public void register(Course c) {
		courses.add(c);
		
	}

	@Override
	public void withdraw(Course c) {
		for (int i = 0; i < courses.size(); i++) { // iterate over the courses 
			if (courses.get(i).getCourse_id().equals(c.getCourse_id())) {
				courses.remove(i);

			}
		}
	}
	
	@Override
	public void view_myCourses() {
		if(this.courses.isEmpty()) {
			System.out.println("You not registerted in any classes yet!");
		}
		else{for(Course c: this.courses) {
			c.printInf();
			}
		}
	}

	@Override
	public ArrayList<Course> get_myCourse() {
		return this.courses;
	}
	
	@Override
	public boolean registered(Course c) {
		boolean registered = false;
		for (int i = 0; i < courses.size(); i++) { // iterate over the courses 
			if (courses.get(i).getCourse_id().equals(c.getCourse_id())) {
				registered = true;
			}
		}
		return registered;
	}
	
	@Override
	public void printInf() {
		System.out.println("First Name: "+ firstName + " Last Name: " + lastName);
	}
	

	@Override
	public int getID() {
		return this.id;
	}
	
	@Override
	public void setID(int id) {
		this.id = id;
	}
	
}
