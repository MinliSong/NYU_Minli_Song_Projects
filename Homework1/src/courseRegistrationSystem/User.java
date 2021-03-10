package courseRegistrationSystem;

import java.io.Serializable;

public abstract class User implements Serializable  {
	String username;
	String passport;
	String firstName;
	String lastName;
	
	public User() {
		this.username = "username";
		this.passport = "passport";
	}
	
	public User(String username, String passport) {
		this.username = username;
		this.passport = passport;
	}
	
	public User(String username, String passport, String firstName, String lastName) {
		this.username=username;
		this.passport=passport;
		this.firstName=firstName;
		this.lastName=lastName;
	}
	public String getUsername() {
		return this.username;
	}
	
	public String getPassport() {
		return this.passport;
	}
	
	public String getFirstname() {
		return this.firstName;
	}
	
	public String getLastname() {
		return this.lastName;
	}
	
	public void setUsername(String username) {
		this.username=username;
	}
	
	public void setPassport(String passport) {
		this.passport=passport;
	}
	
	public void setFirstName(String firstName) {
		this.firstName=firstName;
	}
	
	public void setLastName(String lastName) {
		this.lastName=lastName;
	}
	
}
