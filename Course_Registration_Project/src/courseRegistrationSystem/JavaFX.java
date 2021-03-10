package courseRegistrationSystem;
import java.awt.Insets;
import java.util.ArrayList;
import javafx.scene.control.cell.PropertyValueFactory;

import javafx.scene.text.Text;
import javax.xml.ws.handler.MessageContext.Scope;
import javafx.scene.control.TableColumn;
import javafx.application.*;
import javafx.geometry.Pos;
import javafx.stage.*;
import javafx.scene.*;
import javafx.scene.layout.*;
import javafx.scene.control.*;
import javafx.collections.*;

public class JavaFX extends Application{
	double length = 600;
	double height = 600;
	Stage window;
	Scene beginPage;
	@Override
	public void start(Stage primaryStage) throws Exception {
		window = primaryStage;
		
		GridPane grid = new GridPane();
	    grid.setHgap(10);
	    grid.setVgap(12);
	    
		Label label1 = new Label("Welcome to the course register System");
		
		Button admin = new Button("Admin");
		admin.setMaxWidth(Double.MAX_VALUE);
		admin.setOnAction(e -> window.setScene(adminLogin()));
		
		Button student = new Button("Student");
		student.setOnAction(e -> window.setScene(studentLogin()));
		student.setMaxWidth(Double.MAX_VALUE);
	    
	    grid.add(label1, 0, 0);
	    grid.add(admin, 0, 3);
	    grid.add(student, 0, 4);
		grid.setAlignment(Pos.CENTER);

		
		beginPage = new Scene(grid, length, height);
		
		window.setScene(beginPage);
	    window.setTitle("Course Register System");
	    window.show();
	    
	}
	public Scene studentLogin() {
		GridPane grid = new GridPane();
	    grid.setHgap(10);
	    grid.setVgap(12);
	    
		Button studentExit = new Button("Back to login page");
		studentExit.setOnAction(e -> window.setScene(beginPage));
		studentExit.setMaxWidth(Double.MAX_VALUE);
		
		Label userName = new Label("Username");
		Label password = new Label("Password");
		
		TextField userName_input = new TextField();
		userName_input.setEditable(true);
		
		TextField password_input = new TextField();
		password_input.setEditable(true);
		
		Button login = new Button("Login");
		login.setMaxWidth(Double.MAX_VALUE);
		login.setOnAction(e -> {//Check the username and password 
			String u = userName_input.getText();
			String p = password_input.getText();
			for(Student s: RegistrationSystem.studentList) {
				if(s.getUsername().equalsIgnoreCase(u)&&s.getPassport().equalsIgnoreCase(p)) {
					window.setScene(studentMoel(s));
					break;
				}
				display("Login failed","Username or Passwrod are not correct");
			}
		});
	    
	    grid.add(userName, 0, 0);
	    grid.add(userName_input, 1, 0);
	    grid.add(password, 0, 1);
	    grid.add(password_input, 1, 1);
	    grid.add(login,0,2);
	    grid.add(studentExit, 9, 9);

		grid.setAlignment(Pos.CENTER);
		Scene studentLogin = new Scene(grid, length, height);
		return studentLogin;
	}
	
	private Scene studentMoel(Student s) {
		GridPane grid = new GridPane();
	    grid.setHgap(10);
	    grid.setVgap(12);
		
		Button Exit = new Button("Back to login page");
		Exit.setOnAction(e -> window.setScene(beginPage));
		Exit.setMaxWidth(Double.MAX_VALUE);
		
		Button viewCourses = new Button("View all courses");
		viewCourses.setOnAction(e -> window.setScene(viewAllCourses(s)));
		viewCourses.setMaxWidth(Double.MAX_VALUE);
		
		Button viewFullCourses = new Button("View Full courses");
		viewFullCourses.setOnAction(e -> window.setScene(viewNotFullCourses(s)));
		viewFullCourses.setMaxWidth(Double.MAX_VALUE);
		
		Button registerCourses = new Button("Register courses");
		registerCourses.setOnAction(e -> window.setScene(registerCourses(s)));
		registerCourses.setMaxWidth(Double.MAX_VALUE);
		
		Button withdrawCourses = new Button("Withdraw courses");
		withdrawCourses.setOnAction(e -> window.setScene(withdrawCourses(s)));
		withdrawCourses.setMaxWidth(Double.MAX_VALUE);
		
		Button viewMyCourses = new Button("View my courses");
		viewMyCourses.setOnAction(e -> window.setScene(viewMyCourses(s)));
		viewMyCourses.setMaxWidth(Double.MAX_VALUE);
		
		grid.add(viewCourses, 0, 0);
		grid.add(viewFullCourses, 0, 3);
		grid.add(viewMyCourses, 0, 6);
		grid.add(withdrawCourses, 0, 12);
		grid.add(registerCourses, 0, 9);
		grid.add(Exit, 12, 12);
		
		grid.setAlignment(Pos.CENTER);
		
		Scene studentMoel = new Scene(grid, length, height);
		
		return studentMoel;
		
	}
	
	
	private Scene viewMyCourses(Student s) {
		Button Exit = new Button("Back to Course Management page");
		Exit.setOnAction(e -> window.setScene(studentMoel(s)));
		Exit.setMaxWidth(Double.MAX_VALUE);
		
		ObservableList<Course> courseList = FXCollections.observableArrayList(s.get_myCourse());
		TableView<Course> courses = new TableView<>();
		
		//Name
		TableColumn<Course, String> nameColumn = new TableColumn<>("Name");
        nameColumn.setMinWidth(200);
        nameColumn.setCellValueFactory(new PropertyValueFactory<>("course_name"));

        //Id
        TableColumn<Course, String> idColumn = new TableColumn<>("Id");
        idColumn.setMinWidth(100);
        idColumn.setCellValueFactory(new PropertyValueFactory<>("course_id"));

        //Section
        TableColumn<Course, String> sectionColumn = new TableColumn<>("Section");
        sectionColumn.setMinWidth(100);
        sectionColumn.setCellValueFactory(new PropertyValueFactory<>("section_num"));
        
        //Instructor
        TableColumn<Course, String> instructorColumn = new TableColumn<>("Instructor");
        instructorColumn.setMinWidth(200);
        instructorColumn.setCellValueFactory(new PropertyValueFactory<>("course_instructor"));
        
        //location
        TableColumn<Course, String> locationColumn = new TableColumn<>("Location");
        locationColumn.setMinWidth(100);
        locationColumn.setCellValueFactory(new PropertyValueFactory<>("location"));
        
        //Max Student
        TableColumn<Course, Integer> MaxSColumn = new TableColumn<>("Max Student");
        MaxSColumn.setMinWidth(100);
        MaxSColumn.setCellValueFactory(new PropertyValueFactory<Course, Integer>("max_student"));
        
        //Current Student
        TableColumn<Course, Integer> CurrentSColumn = new TableColumn<>("Current Sutdent");
        CurrentSColumn.setMinWidth(100);
        CurrentSColumn.setCellValueFactory(new PropertyValueFactory<Course, Integer>("current_student"));

        
        //student List
        TableColumn<Course, ArrayList<Student>> studentList = new TableColumn<>("Student List");
        studentList.setMinWidth(100);
        studentList.setCellValueFactory(new PropertyValueFactory<>("register_student"));
        
        courses.setItems(courseList);
        courses.getColumns().addAll(nameColumn, idColumn, sectionColumn, instructorColumn, locationColumn, MaxSColumn, CurrentSColumn);
        
        VBox vBox = new VBox();
        vBox.getChildren().addAll(courses,Exit);
                
        Scene scene = new Scene(vBox,length,height);
        
	
		return scene;
	}
	
	private Scene withdrawCourses(Student s) {
		Button Exit = new Button("Back to Course Management page");
		Exit.setOnAction(e -> window.setScene(studentMoel(s)));
		Exit.setMaxWidth(Double.MAX_VALUE);
		
		ObservableList<Course> courseList = FXCollections.observableArrayList(s.get_myCourse());
		TableView<Course> courses = new TableView<>();
		
		//Name
		TableColumn<Course, String> nameColumn = new TableColumn<>("Name");
        nameColumn.setMinWidth(200);
        nameColumn.setCellValueFactory(new PropertyValueFactory<>("course_name"));

        //Id
        TableColumn<Course, String> idColumn = new TableColumn<>("Id");
        idColumn.setMinWidth(100);
        idColumn.setCellValueFactory(new PropertyValueFactory<>("course_id"));

        //Section
        TableColumn<Course, String> sectionColumn = new TableColumn<>("Section");
        sectionColumn.setMinWidth(100);
        sectionColumn.setCellValueFactory(new PropertyValueFactory<>("section_num"));
        
        //Instructor
        TableColumn<Course, String> instructorColumn = new TableColumn<>("Instructor");
        instructorColumn.setMinWidth(200);
        instructorColumn.setCellValueFactory(new PropertyValueFactory<>("course_instructor"));
        
        //location
        TableColumn<Course, String> locationColumn = new TableColumn<>("Location");
        locationColumn.setMinWidth(100);
        locationColumn.setCellValueFactory(new PropertyValueFactory<>("location"));
        
        //Max Student
        TableColumn<Course, Integer> MaxSColumn = new TableColumn<>("Max Student");
        MaxSColumn.setMinWidth(100);
        MaxSColumn.setCellValueFactory(new PropertyValueFactory<Course, Integer>("max_student"));
        
        //Current Student
        TableColumn<Course, Integer> CurrentSColumn = new TableColumn<>("Current Sutdent");
        CurrentSColumn.setMinWidth(100);
        CurrentSColumn.setCellValueFactory(new PropertyValueFactory<Course, Integer>("current_student"));

        
        //student List
        TableColumn<Course, ArrayList<Student>> studentList = new TableColumn<>("Student List");
        studentList.setMinWidth(100);
        studentList.setCellValueFactory(new PropertyValueFactory<>("register_student"));
              
        courses.setItems(courseList);
        courses.getColumns().addAll(nameColumn, idColumn, sectionColumn, instructorColumn, locationColumn, MaxSColumn, CurrentSColumn);
        
        TextField courseName = new TextField();
        courseName.setPromptText("Course Name");
        courseName.setMinWidth(length/6);
        
        TextField courseSection = new TextField();
        courseSection.setPromptText("Course Section");
        courseSection.setMinWidth(length/6);
        
        TextField studentFirstName = new TextField();
        studentFirstName.setPromptText("Student First Name");
        studentFirstName.setMinWidth(length/6);
        
        TextField studentLastName = new TextField();
        studentLastName.setPromptText("Student Last Name");
        studentLastName.setMinWidth(length/6);
        
        Button withdraw = new Button("Withdraw");
        withdraw.setMinWidth(length/6);
        
        HBox hbox = new HBox();
        // hbox.setPadding(new Insets(10,10,10,10));
        hbox.setSpacing(10);
        hbox.getChildren().addAll(courseName,courseSection, studentFirstName, studentLastName, withdraw);
         
         
        VBox vBox = new VBox();
        vBox.getChildren().addAll(courses,hbox,Exit);
        
        
        withdraw.setOnAction(e ->{
        	Course c = RegistrationSystem.findCourse(courseName.getText(), courseSection.getText());
        	if(s.get_myCourse().isEmpty()) {
        		Text tx = new Text("You are not enrolled in any class right now "  );
				vBox.getChildren().addAll(tx);
        	}
        	else{
        		if(c!=null) {
            		if(s.registered(c)) {
            			s.withdraw(c);
    					c.withdrawStudent(s);
    					Text tx = new Text("You successfully withdraw in class: " +courseName.getText() );
    					vBox.getChildren().addAll(tx);
            		}
            		else {
            			Text tx = new Text("You are not in class : " +courseName.getText());
            			vBox.getChildren().addAll(tx);
            		}
            	}
            	else {
            		Text tx = new Text("Cannot find the course: " + courseName.getText());
        			vBox.getChildren().addAll(tx);
            	}
        	}
        });
        
        Scene scene = new Scene(vBox,length,height);        
	
		return scene;
	}
	
	private Scene registerCourses(Student s) {

		Button Exit = new Button("Back to Course Management page");
		Exit.setOnAction(e -> window.setScene(studentMoel(s)));
		Exit.setMaxWidth(Double.MAX_VALUE);
		
		ObservableList<Course> courseList = FXCollections.observableArrayList(RegistrationSystem.courseList);
		TableView<Course> courses = new TableView<>();
		
		//Name
		TableColumn<Course, String> nameColumn = new TableColumn<>("Name");
        nameColumn.setMinWidth(200);
        nameColumn.setCellValueFactory(new PropertyValueFactory<>("course_name"));

        //Id
        TableColumn<Course, String> idColumn = new TableColumn<>("Id");
        idColumn.setMinWidth(100);
        idColumn.setCellValueFactory(new PropertyValueFactory<>("course_id"));

        //Section
        TableColumn<Course, String> sectionColumn = new TableColumn<>("Section");
        sectionColumn.setMinWidth(100);
        sectionColumn.setCellValueFactory(new PropertyValueFactory<>("section_num"));
        
        //Instructor
        TableColumn<Course, String> instructorColumn = new TableColumn<>("Instructor");
        instructorColumn.setMinWidth(200);
        instructorColumn.setCellValueFactory(new PropertyValueFactory<>("course_instructor"));
        
        //location
        TableColumn<Course, String> locationColumn = new TableColumn<>("Location");
        locationColumn.setMinWidth(100);
        locationColumn.setCellValueFactory(new PropertyValueFactory<>("location"));
        
        //Max Student
        TableColumn<Course, Integer> MaxSColumn = new TableColumn<>("Max Student");
        MaxSColumn.setMinWidth(100);
        MaxSColumn.setCellValueFactory(new PropertyValueFactory<Course, Integer>("max_student"));
        
        //Current Student
        TableColumn<Course, Integer> CurrentSColumn = new TableColumn<>("Current Sutdent");
        CurrentSColumn.setMinWidth(100);
        CurrentSColumn.setCellValueFactory(new PropertyValueFactory<Course, Integer>("current_student"));

        
        //student List
        TableColumn<Course, ArrayList<Student>> studentList = new TableColumn<>("Student List");
        studentList.setMinWidth(100);
        studentList.setCellValueFactory(new PropertyValueFactory<>("register_student"));
              
        courses.setItems(courseList);
        courses.getColumns().addAll(nameColumn, idColumn, sectionColumn, instructorColumn, locationColumn, MaxSColumn, CurrentSColumn);
        
        TextField courseName = new TextField();
        courseName.setPromptText("Course Name");
        courseName.setMinWidth(length/6);
        
        TextField courseSection = new TextField();
        courseSection.setPromptText("Course Section");
        courseSection.setMinWidth(length/6);
        
        TextField studentFirstName = new TextField();
        studentFirstName.setPromptText("Student First Name");
        studentFirstName.setMinWidth(length/6);
        
        TextField studentLastName = new TextField();
        studentLastName.setPromptText("Student Last Name");
        studentLastName.setMinWidth(length/6);
        
        Button register = new Button("Register");
        register.setMinWidth(length/6);
        
        HBox hbox = new HBox();
        // hbox.setPadding(new Insets(10,10,10,10));
         hbox.setSpacing(10);
         hbox.getChildren().addAll(courseName,courseSection, studentFirstName, studentLastName, register);
         
         
         VBox vBox = new VBox();
         vBox.getChildren().addAll(courses,hbox,Exit);
        
        
        register.setOnAction(e ->{
        	Course c = RegistrationSystem.findCourse(courseName.getText(), courseSection.getText());
        	if(c!=null) {
        		if(!s.registered(c)) {
					s.register(c);
					c.register(s);
					Text tx = new Text("You successfully enrolled in class: " +courseName.getText() );
					vBox.getChildren().addAll(tx);
        		}
        		else {
        			Text tx = new Text("You have already enrolled in class: " +courseName.getText());
        			vBox.getChildren().addAll(tx);
        		}
        	}
        	else {
        		Text tx = new Text("Cannot find the course: " + courseName.getText());
    			vBox.getChildren().addAll(tx);
        	}
        });
        
        Scene scene = new Scene(vBox,length,height);        
	
		return scene;
	}
	
	private Scene viewNotFullCourses(Student s) {
		
		Button Exit = new Button("Back to Course Management page");
		Exit.setOnAction(e -> window.setScene(studentMoel(s)));
		Exit.setMaxWidth(Double.MAX_VALUE);
		ArrayList<Course> noneFullList = new ArrayList<>();
		for(Course c:RegistrationSystem.courseList) {
			if(c.getMax_student()>c.getCurrent_student()) {
				noneFullList.add(c);
			}
		}
		ObservableList<Course> courseList = FXCollections.observableArrayList(noneFullList);
		TableView<Course> courses = new TableView<>();
		
		//Name
		TableColumn<Course, String> nameColumn = new TableColumn<>("Name");
        nameColumn.setMinWidth(200);
        nameColumn.setCellValueFactory(new PropertyValueFactory<>("course_name"));

        //Id
        TableColumn<Course, String> idColumn = new TableColumn<>("Id");
        idColumn.setMinWidth(100);
        idColumn.setCellValueFactory(new PropertyValueFactory<>("course_id"));

        //Section
        TableColumn<Course, String> sectionColumn = new TableColumn<>("Section");
        sectionColumn.setMinWidth(100);
        sectionColumn.setCellValueFactory(new PropertyValueFactory<>("section_num"));
        
        //Instructor
        TableColumn<Course, String> instructorColumn = new TableColumn<>("Instructor");
        instructorColumn.setMinWidth(200);
        instructorColumn.setCellValueFactory(new PropertyValueFactory<>("course_instructor"));
        
        //location
        TableColumn<Course, String> locationColumn = new TableColumn<>("Location");
        locationColumn.setMinWidth(100);
        locationColumn.setCellValueFactory(new PropertyValueFactory<>("location"));
        
        //Max Student
        TableColumn<Course, Integer> MaxSColumn = new TableColumn<>("Max Student");
        MaxSColumn.setMinWidth(100);
        MaxSColumn.setCellValueFactory(new PropertyValueFactory<Course, Integer>("max_student"));
        
        //Current Student
        TableColumn<Course, Integer> CurrentSColumn = new TableColumn<>("Current Sutdent");
        CurrentSColumn.setMinWidth(100);
        CurrentSColumn.setCellValueFactory(new PropertyValueFactory<Course, Integer>("current_student"));

        
        //student List
        TableColumn<Course, ArrayList<Student>> studentList = new TableColumn<>("Student List");
        studentList.setMinWidth(100);
        studentList.setCellValueFactory(new PropertyValueFactory<>("register_student"));
               
        courses.setItems(courseList);
        courses.getColumns().addAll(nameColumn, idColumn, sectionColumn, instructorColumn, locationColumn, MaxSColumn, CurrentSColumn);
        
        VBox vBox = new VBox();
        vBox.getChildren().addAll(courses,Exit);
              
        Scene scene = new Scene(vBox,length,height);
		return scene;
	}
	
	private Scene viewAllCourses(Student s) {

		Button Exit = new Button("Back to Course Management page");
		Exit.setOnAction(e -> window.setScene(studentMoel(s)));
		Exit.setMaxWidth(Double.MAX_VALUE);
		
		ObservableList<Course> courseList = FXCollections.observableArrayList(RegistrationSystem.courseList);
		TableView<Course> courses = new TableView<>();
		
		//Name
		TableColumn<Course, String> nameColumn = new TableColumn<>("Name");
        nameColumn.setMinWidth(200);
        nameColumn.setCellValueFactory(new PropertyValueFactory<>("course_name"));

        //Id
        TableColumn<Course, String> idColumn = new TableColumn<>("Id");
        idColumn.setMinWidth(100);
        idColumn.setCellValueFactory(new PropertyValueFactory<>("course_id"));

        //Section
        TableColumn<Course, String> sectionColumn = new TableColumn<>("Section");
        sectionColumn.setMinWidth(100);
        sectionColumn.setCellValueFactory(new PropertyValueFactory<>("section_num"));
        
        //Instructor
        TableColumn<Course, String> instructorColumn = new TableColumn<>("Instructor");
        instructorColumn.setMinWidth(200);
        instructorColumn.setCellValueFactory(new PropertyValueFactory<>("course_instructor"));
        
        //location
        TableColumn<Course, String> locationColumn = new TableColumn<>("Location");
        locationColumn.setMinWidth(100);
        locationColumn.setCellValueFactory(new PropertyValueFactory<>("location"));
        
        //Max Student
        TableColumn<Course, Integer> MaxSColumn = new TableColumn<>("Max Student");
        MaxSColumn.setMinWidth(100);
        MaxSColumn.setCellValueFactory(new PropertyValueFactory<Course, Integer>("max_student"));
        
        //Current Student
        TableColumn<Course, Integer> CurrentSColumn = new TableColumn<>("Current Sutdent");
        CurrentSColumn.setMinWidth(100);
        CurrentSColumn.setCellValueFactory(new PropertyValueFactory<Course, Integer>("current_student"));

        
        //student List
        TableColumn<Course, ArrayList<Student>> studentList = new TableColumn<>("Student List");
        studentList.setMinWidth(100);
        studentList.setCellValueFactory(new PropertyValueFactory<>("register_student"));
              
        courses.setItems(courseList);
        courses.getColumns().addAll(nameColumn, idColumn, sectionColumn, instructorColumn, locationColumn, MaxSColumn, CurrentSColumn);
        
        VBox vBox = new VBox();
        vBox.getChildren().addAll(courses,Exit);
              
        Scene scene = new Scene(vBox,length,height);        
	
		return scene;
	}
	
	
	public Scene adminLogin() {
		GridPane grid = new GridPane();
	    grid.setHgap(10);
	    grid.setVgap(12);
	    
		Button adminExit = new Button("Back to login page");
		adminExit.setOnAction(e -> window.setScene(beginPage));	
		adminExit.setMaxWidth(Double.MAX_VALUE);
		
		Label userName = new Label("Username");
		Label password = new Label("Password");
		
		TextField userName_input = new TextField();
		userName_input.setEditable(true);
		
		TextField password_input = new TextField();
		password_input.setEditable(true);
		
		Button login = new Button("Login");
		login.setMaxWidth(Double.MAX_VALUE);
		login.setOnAction(e -> {//Check the username and password 
			String u = userName_input.getText();
			String p = password_input.getText();
			for(Admin a: RegistrationSystem.adminList) {
				if(a.getUsername().equalsIgnoreCase(u)&&a.getPassport().equalsIgnoreCase(p)) {
					window.setScene(adminModel(a));
					break;
				}
			}
			display("Login failed","Username or Passwrod are not correct");
			
		});
		    
		VBox vb1 = new VBox(20);
		vb1.getChildren().addAll(login,adminExit);
		
		grid.add(userName, 0, 0);
		grid.add(userName_input, 1, 0);
		grid.add(password, 0, 1);
		grid.add(password_input, 1, 1);
		grid.add(vb1, 0, 2, 2, 1);
	
		grid.setAlignment(Pos.CENTER);
		Scene adminLogin = new Scene(grid, height, height);
		return adminLogin;
	}
	
    private Scene adminModel(Admin a) {
		// TODO Auto-generated method stub
		return null;
	}
    
	public static void display(String title, String message) {
        Stage window = new Stage();

        //Block events to other windows
        window.initModality(Modality.APPLICATION_MODAL);
        window.setTitle(title);
        window.setMinWidth(250);

        Label label = new Label();
        label.setText(message);
        Button closeButton = new Button("Close this window");
        closeButton.setOnAction(e -> window.close());

        VBox layout = new VBox(10);
        layout.getChildren().addAll(label, closeButton);
        layout.setAlignment(Pos.CENTER);

        //Display window and wait for it to be closed before returning
        Scene scene = new Scene(layout);
        window.setScene(scene);
        window.showAndWait();
    }
    
	
}
