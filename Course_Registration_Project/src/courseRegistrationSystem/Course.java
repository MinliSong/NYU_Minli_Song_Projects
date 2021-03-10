package courseRegistrationSystem;

import java.io.Serializable;
import java.util.ArrayList;

public class Course implements Comparable<Course>, Serializable{
	private String course_name;
	private String course_id;
	private int max_student;
	private int current_student;
	private ArrayList<Student> register_student = new ArrayList<Student>();
	private String course_instructor;
	private String section_num;
	private String location;
	
	public Course(String course_name, String course_id) {
		this.course_name = course_name;
		this.course_id = course_id;
	}
	
	public Course(String course_name, String course_id, int max_student, int current_student, ArrayList<Student> register_student_name, String course_instructor, String section_num, String location) {
		this.course_name = course_name;
		this.course_id = course_id;
		this.max_student = max_student;
		this.current_student = current_student;
		this.register_student = register_student_name;
		this.course_instructor = course_instructor;
		this.section_num = section_num;
		this.location = location;
	}
	
	public ArrayList<Student> get_studentList() {
		return this.register_student;
	}
	
	public Integer getCurrent_student() {
		return this.current_student;
	}
	
	public void set_currentStudent(int current_studentNum) {
		this.current_student = current_studentNum;
	}
	
	public void setCourse_instructor(String course_instructor) {
		this.course_instructor = course_instructor;
	}
	
	public String getCourse_instructor() {
		return this.course_instructor;
	}
	
	public void setSection_num(String section_num) {
		this.section_num = section_num;
	}
	
	public String getSection_num() {
		return this.section_num;
	}
	
	public void setLocation(String location) {
		this.location = location;
	}
	
	public String getLocation() {
		return this.location;
	}
	
	public void setCourse_name(String course_name) {
		this.course_name = course_name;
	}
	
	public String getCourse_name() {
		return this.course_name;
	}
	
	public void setCourse_id(String course_id) {
		this.course_id = course_id;
	}
	
	public String getCourse_id() {
		return this.course_id;
	}
	
	public void setMax_student(int max_student) {
		this.max_student = max_student;
	}
	
	public Integer getMax_student() {
		return this.max_student;
	}
	
	public void register(Student s) {
		this.register_student.add(s);
		this.current_student++;
	}
	
	public void withdrawStudent(Student s) {
		this.register_student.remove(s);
		this.current_student--;
	}

	@Override
	public int compareTo(Course o) {
		return o.getCurrent_student();
	}
	
	public void printInf() {
		System.out.println("Course Name: " + course_name + " Course ID: " +course_id + " Max Student: "+ max_student + " Current Student: " + current_student + " Course Instructor: " +  course_instructor + " Section number: " + section_num + " Location: " + location);
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((course_id == null) ? 0 : course_id.hashCode());
		result = prime * result + ((course_instructor == null) ? 0 : course_instructor.hashCode());
		result = prime * result + ((course_name == null) ? 0 : course_name.hashCode());
		result = prime * result + current_student;
		result = prime * result + ((location == null) ? 0 : location.hashCode());
		result = prime * result + max_student;
		result = prime * result + ((register_student == null) ? 0 : register_student.hashCode());
		result = prime * result + ((section_num == null) ? 0 : section_num.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) { //rewrite equal methods
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Course other = (Course) obj;
		if (course_id == null) {
			if (other.course_id != null)
				return false;
		} else if (!course_id.equals(other.course_id))
			return false;
		if (course_instructor == null) {
			if (other.course_instructor != null)
				return false;
		} else if (!course_instructor.equals(other.course_instructor))
			return false;
		if (course_name == null) {
			if (other.course_name != null)
				return false;
		} else if (!course_name.equals(other.course_name))
			return false;
		if (current_student != other.current_student)
			return false;
		if (location == null) {
			if (other.location != null)
				return false;
		} else if (!location.equals(other.location))
			return false;
		if (max_student != other.max_student)
			return false;
		if (register_student == null) {
			if (other.register_student != null)
				return false;
		} else if (!register_student.equals(other.register_student))
			return false;
		if (section_num == null) {
			if (other.section_num != null)
				return false;
		} else if (!section_num.equals(other.section_num))
			return false;
		return true;
	}
	
	
}
