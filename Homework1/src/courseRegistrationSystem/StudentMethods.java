package courseRegistrationSystem;

import java.util.ArrayList;

public interface StudentMethods {

	public int getID();

	public void setID(int id);
	
	public void register(Course c);
	
	public void withdraw(Course c);
	
	public void view_myCourses();
	
	public boolean registered(Course c);
	
	public void printInf();

	public ArrayList<Course> get_myCourse();
	

}
