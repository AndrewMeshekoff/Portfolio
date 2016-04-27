/****************************************************************************
*	Alex Fuller				afull004
*	Andrew Meshekoff			amesh001
*
*****************************************************************************/	

/*
 * Template JAVA User Interface
 * =============================
 *
 * Database Management Systems
 * Department of Computer Science &amp; Engineering
 * University of California - Riverside
 *
 * Target DBMS: 'Postgres'
 *
 */


import java.sql.DriverManager;
import java.sql.Connection;
import java.sql.Statement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.io.File;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.List;
import java.util.ArrayList;

/**
 * This class defines a simple embedded SQL utility class that is designed to
 * work with PostgreSQL JDBC drivers.
 *
 */
public class ProfNetwork {

   // reference to physical database connection.
   private Connection _connection = null;

   // handling the keyboard inputs through a BufferedReader
   // This variable can be global for convenience.
   static BufferedReader in = new BufferedReader(
                                new InputStreamReader(System.in));

   /**
    * Creates a new instance of ProfNetwork
    *
    * @param hostname the MySQL or PostgreSQL server hostname
    * @param database the name of the database
    * @param username the user name used to login to the database
    * @param password the user login password
    * @throws java.sql.SQLException when failed to make a connection.
    */
   public ProfNetwork (String dbname, String dbport, String user, String passwd) throws SQLException {

      System.out.print("Connecting to database...");
      try{
         // constructs the connection URL
         String url = "jdbc:postgresql://localhost:" + dbport + "/" + dbname;
         System.out.println ("Connection URL: " + url + "\n");

         // obtain a physical connection
         this._connection = DriverManager.getConnection(url, user, passwd);
         System.out.println("Done");
      }catch (Exception e){
         System.err.println("Error - Unable to Connect to Database: " + e.getMessage() );
         System.out.println("Make sure you started postgres on this machine");
         System.exit(-1);
      }//end catch
   }//end ProfNetwork

   /**
    * Method to execute an update SQL statement.  Update SQL instructions
    * includes CREATE, INSERT, UPDATE, DELETE, and DROP.
    *
    * @param sql the input SQL string
    * @throws java.sql.SQLException when update failed
    */
   public void executeUpdate (String sql) throws SQLException {
      // creates a statement object
      Statement stmt = this._connection.createStatement ();

      // issues the update instruction
      stmt.executeUpdate (sql);

      // close the instruction
      stmt.close ();
   }//end executeUpdate

   /**
    * Method to execute an input query SQL instruction (i.e. SELECT).  This
    * method issues the query to the DBMS and outputs the results to
    * standard out.
    *
    * @param query the input query string
    * @return the number of rows returned
    * @throws java.sql.SQLException when failed to execute the query
    */
   public int executeQueryAndPrintResult (String query) throws SQLException {
      // creates a statement object
      Statement stmt = this._connection.createStatement ();

      // issues the query instruction
      ResultSet rs = stmt.executeQuery (query);

      /*
       ** obtains the metadata object for the returned result set.  The metadata
       ** contains row and column info.
       */
      ResultSetMetaData rsmd = rs.getMetaData ();
      int numCol = rsmd.getColumnCount ();
      int rowCount = 0;

      // iterates through the result set and output them to standard out.
      boolean outputHeader = true;
      while (rs.next()){
	 if(outputHeader){
	    for(int i = 1; i <= numCol; i++){
		System.out.print(rsmd.getColumnName(i) + "\t");
	    }
	    System.out.println();
	    outputHeader = false;
	 }
         for (int i=1; i<=numCol; ++i)
            System.out.print (rs.getString (i) + "\t");
         System.out.println ();
         ++rowCount;
      }//end while
      stmt.close ();
      return rowCount;
   }//end executeQuery

   /**
    * Method to execute an input query SQL instruction (i.e. SELECT).  This
    * method issues the query to the DBMS and returns the results as
    * a list of records. Each record in turn is a list of attribute values
    *
    * @param query the input query string
    * @return the query result as a list of records
    * @throws java.sql.SQLException when failed to execute the query
    */
   public List<List<String>> executeQueryAndReturnResult (String query) throws SQLException {
      // creates a statement object
      Statement stmt = this._connection.createStatement ();

      // issues the query instruction
      ResultSet rs = stmt.executeQuery (query);

      /*
       ** obtains the metadata object for the returned result set.  The metadata
       ** contains row and column info.
       */
      ResultSetMetaData rsmd = rs.getMetaData ();
      int numCol = rsmd.getColumnCount ();
      int rowCount = 0;

      // iterates through the result set and saves the data returned by the query.
      boolean outputHeader = false;
      List<List<String>> result  = new ArrayList<List<String>>();
      while (rs.next()){
          List<String> record = new ArrayList<String>();
         for (int i=1; i<=numCol; ++i)
            record.add(rs.getString (i));
         result.add(record);
      }//end while
      stmt.close ();
      return result;
   }//end executeQueryAndReturnResult

   /**
    * Method to execute an input query SQL instruction (i.e. SELECT).  This
    * method issues the query to the DBMS and returns the number of results
    *
    * @param query the input query string
    * @return the number of rows returned
    * @throws java.sql.SQLException when failed to execute the query
    */
   public int executeQuery (String query) throws SQLException {
       // creates a statement object
       Statement stmt = this._connection.createStatement ();

       // issues the query instruction
       ResultSet rs = stmt.executeQuery (query);

       int rowCount = 0;

       // iterates through the result set and count nuber of results.
       if(rs.next()){
          rowCount++;
       }//end while
       stmt.close ();
       return rowCount;
   }

   /**
    * Method to fetch the last value from sequence. This
    * method issues the query to the DBMS and returns the current
    * value of sequence used for autogenerated keys
    *
    * @param sequence name of the DB sequence
    * @return current value of a sequence
    * @throws java.sql.SQLException when failed to execute the query
    */
   public int getCurrSeqVal(String sequence) throws SQLException {
	Statement stmt = this._connection.createStatement ();

	ResultSet rs = stmt.executeQuery (String.format("Select currval('%s')", sequence));
	if (rs.next())
		return rs.getInt(1);
	return -1;
   }

   /**
    * Method to close the physical connection if it is open.
    */
   public void cleanup(){
      try{
         if (this._connection != null){
            this._connection.close ();
         }//end if
      }catch (SQLException e){
         // ignored.
      }//end try
   }//end cleanup

   /**
    * The main execution method
    *
    * @param args the command line arguments this inclues the <mysql|pgsql> <login file>
    */
   public static void main (String[] args) {
      if (args.length != 3) {
         System.err.println (
            "Usage: " +
            "java [-classpath <classpath>] " +
            ProfNetwork.class.getName () +
            " <dbname> <port> <user>");
         return;
      }//end if

      Greeting();
      ProfNetwork esql = null;
      try{
         // use postgres JDBC driver.
         Class.forName ("org.postgresql.Driver").newInstance ();
         // instantiate the ProfNetwork object and creates a physical
         // connection.
         String dbname = args[0];
         String dbport = args[1];
         String user = args[2];
         esql = new ProfNetwork (dbname, dbport, user, "");

         boolean keepon = true;
         while(keepon) {
            // These are sample SQL statements
            System.out.println("MAIN MENU");
            System.out.println("---------");
            System.out.println("1. Create user");
            System.out.println("2. Log in");
            System.out.println("9. < EXIT");
            String authorisedUser = null;
            switch (readChoice()){
               case 1: CreateUser(esql); break;
               case 2: authorisedUser = LogIn(esql); break;
               case 9: keepon = false; break;
               default : System.out.println("Unrecognized choice!"); break;
            }//end switch
            if (authorisedUser != null) {
              boolean usermenu = true;
              while(usermenu) {
                System.out.println("MAIN MENU");
                System.out.println("---------");
                System.out.println("1. Goto Friend List");
                System.out.println("2. Update Profile");
                System.out.println("3. Write a new message");
	        System.out.println("4. Send Friend Request");
		System.out.println("5. Search People");
                System.out.println(".........................");
                System.out.println("9. Log out");
                switch (readChoice()){
                   case 1: FriendList(esql, authorisedUser); break;
                   case 2: UpdateProfile(esql, authorisedUser); break;
                   case 3: NewMessage(esql, authorisedUser); break;
                   case 4: SendRequest(esql, authorisedUser); break;
		   case 5: searchPeople(esql, authorisedUser); break;
                   case 9: usermenu = false; break;
                   default : System.out.println("Unrecognized choice!"); break;
                }
              }
            }
         }//end while
      }catch(Exception e) {
         System.err.println (e.getMessage ());
      }finally{
         // make sure to cleanup the created table and close the connection.
         try{
            if(esql != null) {
               System.out.print("Disconnecting from database...");
               esql.cleanup ();
               System.out.println("Done\n\nBye !");
            }//end if
         }catch (Exception e) {
            // ignored.
         }//end try
      }//end try
   }//end main

   public static void Greeting(){
      System.out.println(
         "\n\n*******************************************************\n" +
         "              User Interface      	               \n" +
         "*******************************************************\n");
   }//end Greeting

   /*
    * Reads the users choice given from the keyboard
    * @int
    **/
   public static int readChoice() {
      int input;
      // returns only if a correct value is given.
      do {
         System.out.print("Please make your choice: ");
         try { // read the integer, parse it and break.
            input = Integer.parseInt(in.readLine());
            break;
         }catch (Exception e) {
            System.out.println("Your input is invalid!");
            continue;
         }//end try
      }while (true);
      return input;
   }//end readChoice

   /*
    * Creates a new user with privided login, passowrd and phoneNum
    * An empty block and contact list would be generated and associated with a user
    **/
   public static void CreateUser(ProfNetwork esql){
      try{
         System.out.print("\tEnter user login: ");
         String login = in.readLine();
         System.out.print("\tEnter user password: ");
         char [] passwordHidden = System.console().readPassword();
	 String password = new String(passwordHidden);
         System.out.print("\tEnter user email: ");
         String email = in.readLine();
	 System.out.print("\tEnter your name: ");
	 String name = in.readLine();
	 System.out.print("\tEnter your Date of Birth: ");
	 String dateOfBirth = in.readLine();

	 //Creating empty contact\block lists for a user
	 String query = String.format("INSERT INTO USR (userId, password, email, name, dateOfBirth) VALUES ('%s','%s','%s', '%s', '%s')", login, password, email, name, dateOfBirth);

         esql.executeUpdate(query);
         System.out.println ("User successfully created!");
      }catch(Exception e){
         System.err.println (e.getMessage ());
      }
   }//end

   /*
    * Check log in credentials for an existing user
    * @return User login or null is the user does not exist
    **/
   public static String LogIn(ProfNetwork esql){
      try{
         System.out.print("\tEnter user login: ");
         String login = in.readLine();
         System.out.print("\tEnter user password: ");
         String password = in.readLine();

         String query = String.format("SELECT * FROM USR WHERE userId = '%s' AND password = '%s'", login, password);
         int userNum = esql.executeQuery(query);
	 if (userNum > 0)
		return login;
         return null;
      }catch(Exception e){
         System.err.println (e.getMessage ());
         return null;
      }
   }//end

// Rest of the functions definition go in here
   public static void UpdateProfile(ProfNetwork esql, String login){
	String query = "";
	String company = "";
	String role = "";
	String location = "";
	String startDate = "";
	String endDate = "";
       try{
	 boolean updateProf = true;
	 //String query;
	 while(updateProf){
	 System.out.println(" -------UPDATE PROFILE MENU---------");
	 System.out.println("1. Change password");
	 System.out.println("2. Add/Update Work Experience" );
	 System.out.println("3. Add/Update Educational Details");
	 System.out.println("9. Exit Update Profile");
	 switch(readChoice()){
		case 1:
			System.out.print("\tTo change passwords first enter old password: ");
			char [] passwordHidden = System.console().readPassword();
			String password = new String(passwordHidden); 
			query = String.format("SELECT * FROM USR WHERE userId ='%s' AND password = '%s'", login, password);
			int userNum = esql.executeQuery(query);
			if(userNum > 0){
				System.out.print("\tEnter new password: ");
				String newPassword = in.readLine();
				query = String.format("UPDATE USR SET password = '%s' WHERE userId = '%s'", newPassword, login);
				esql.executeUpdate(query);
	 		}
			else{
				System.out.println("Incorrect Password!");
				System.out.println(" Returning to Menu");
				UpdateProfile(esql, login);
			};
		break;
		case 2:
			List<List<String>> result = new ArrayList<List<String>>();
			query = String.format("SELECT * FROM WORK_EXPR WHERE userId = '%s'", login);
			result = esql.executeQueryAndReturnResult(query);
			int result2 = esql.executeQueryAndPrintResult(query);
			if(result.isEmpty()){
				System.out.println("NO RESULT");
			}
			else{
			System.out.println("1. Add Work Experience");
			System.out.println("2. Update Work Experience");
			System.out.println("9. Return to Menu");
			switch(readChoice()){
				case 1:
				System.out.print("\t Enter company name: ");
				company = in.readLine();
				System.out.print("\t Enter role/position: ");
				role = in.readLine();
				System.out.print("\t Enter the work location: ");
				location = in.readLine();
				System.out.print("\t Enter start date: ");
				startDate = in.readLine();
				System.out.print("\t Enter end date: ");
				endDate = in.readLine();
				query = String.format("INSERT INTO WORK_EXPR (userId, company, role, location, startDate, endDate) " + "VALUES('%s', '%s', '%s', '%s', '%s', '%s')", login, company, role, location, startDate, endDate);
				esql.executeUpdate(query);
				break;
				case 2:
				System.out.println("Enter which work experience to update");
				int workChoice = 0;
				workChoice = Integer.parseInt(esql.in.readLine().trim()) - 1;
				System.out.println("1. Update company");
				System.out.println("2. Update role");
				System.out.println("3. Update location");
				System.out.println("4. Update start date");
				System.out.println("5. Update end date");
				switch(esql.readChoice()){
					case 1:
					System.out.println("Enter company name: ");
					company = in.readLine();
					query = String.format("UPDATE WORK_EXPR SET company = '%s' WHERE userId = '%s' AND company = '%s' AND role = '%s'AND startDate = '%s'", company, login, result.get(workChoice).get(1), result.get(workChoice).get(2), result.get(workChoice).get(4));
                                esql.executeUpdate(query);
					break;
					case 2:
					System.out.println("Enter role: ");
					role = in.readLine();
					query = String.format("UPDATE WORK_EXPR SET role = '%s' WHERE userId = '%s' AND company = '%s' AND role = '%s'AND startDate = '%s'", role, login, result.get(workChoice).get(1), result.get(workChoice).get(2), result.get(workChoice).get(4));
					esql.executeUpdate(query);
					break;
					case 3:
					System.out.println("Enter location: ");
					location = in.readLine();
					query = String.format("UPDATE WORK_EXPR SET location = '%s' WHERE userId = '%s' AND company = '%s' AND role = '%s'AND startDate = '%s'", location, login, result.get(workChoice).get(1), result.get(workChoice).get(2), result.get(workChoice).get(4));
                                	esql.executeUpdate(query);
					break;
					case 4:
					System.out.println("Enter start date: ");
					startDate = in.readLine();
					query = String.format("UPDATE WORK_EXPR SET startDate = '%s' WHERE userId = '%s' AND company = '%s' AND role = '%s'AND startDate = '%s'", startDate, login, result.get(workChoice).get(1), result.get(workChoice).get(2), result.get(workChoice).get(4));
                                	esql.executeUpdate(query);
					break;
					case 5:
					System.out.println("Enter end date: ");
					endDate = in.readLine();
					query = String.format("UPDATE WORK_EXPR SET endDate = '%s' WHERE userId = '%s' AND company = '%s' AND role = '%s'AND startDate = '%s'", endDate, login, result.get(workChoice).get(1), result.get(workChoice).get(2), result.get(workChoice).get(4));
                                	esql.executeUpdate(query);
					break;
					default: break;
				}
				break;
				case 9: UpdateProfile(esql, login);
				break;
				default: break;
				}
			
		break;

		case 3:
			List<List<String>> result = new ArrayList<List<String>>();
			query = String.format("SELECT * FROM WORK_EXPR WHERE userId = '%s'", login);
			result = esql.executeQueryAndReturnResult(query);
			int result2 = esql.executeQueryAndPrintResult(query);
			if(result.isEmpty()){
				System.out.println("NO RESULT");
			}
			else{
			System.out.println("1. Add Education Details");
			System.out.println("2. Update Education Detials");
			System.out.println("9. Return to Menu");
			switch(readChoice()){

			case 1: 
			System.out.print("\t EnterFFFFF institution name: ");
			institutionName = in.readLine();
			System.out.print("\t Enter major: ");
			major = in.readLine();
			System.out.print("\t Enter degree: ");
			degree = in.readLine();
			System.out.print("\t Enter start date: ");
			startdate = in.readLine();
			System.out.print("\t Enter end date: ");
			enddate = in.readLine();
				query = String.format("INSERT INTO EDUCTIONAL_DETAILS (userId, institutionName, major, degree, startdate, enddate) " + "VALUES('%s', '%s', '%s', '%s', '%s', '%s')", login, institutionName, major, degree, startdate, enddate);
				esql.executeUpdate(query);
			break;

			case 2:

			System.out.println("Enter which work experience to update");
			int edChoice = 0;
			edChoice = Integer.parseInt(esql.in.readLine().trim()) - 1;
			System.out.println("1. Update institutionName");
			System.out.println("2. Update major");
			System.out.println("3. Update degree");
			System.out.println("4. Update start date");					System.out.println("5. Update end date");
			switch(esql.readChoice()){
				case 1:
				System.out.println("Enter institution name: ");
				institutionName = in.readLine();
				query = String.format("UPDATE EDUCATIONAL_DETAILS institutionName = '%s' WHERE userId = '%s' AND major = '%s' AND  = '%s'AND degree = '%s'", institutionName, login, result.get(edChoice).get(2), result.get(workChoice).get(3));
                                esql.executeUpdate(query);
				break;
				case 2:
				System.out.println("Enter major: ");
				major = in.readLine();
				query = String.format("UPDATE EDUCATIONAL_DETAILS major = '%s' WHERE userId = '%s' AND major = '%s' AND  = '%s'AND degree = '%s'", major, login, result.get(edChoice).get(2), result.get(workChoice).get(3));

				esql.executeUpdate(query);
				break;
				case 3:
				System.out.println("Enter degree: ");
				degree = in.readLine();
				query = String.format("UPDATE EDUCATIONAL_DETAILS degree = '%s' WHERE userId = '%s' AND major = '%s' AND  = '%s'AND degree = '%s'", degree, login, result.get(edChoice).get(2), result.get(workChoice).get(3));
			
				esql.executeUpdate(query);
				break;
				case 4:
				System.out.println("Enter start date: ");
				startdate = in.readLine();
				query = String.format("UPDATE EDUCATIONAL_DETAILS startdate = '%s' WHERE userId = '%s' AND major = '%s' AND  = '%s'AND degree = '%s'", startdate, login, result.get(edChoice).get(2), result.get(workChoice).get(3));

				esql.executeUpdate(query);
				break;
				case 5:
				System.out.println("Enter end date: ");
				enddate = in.readLine();
				query = String.format("UPDATE EDUCATIONAL_DETAILS enddate = '%s' WHERE userId = '%s' AND major = '%s' AND  = '%s'AND degree = '%s'", enddate, login, result.get(edChoice).get(2), result.get(workChoice).get(3));

				esql.executeUpdate(query);
				break;
				default: break;
				}
			break;
			case 9: UpdateProfile(esql, login);
			break;
			default: break;
			}
			
		break;
		case 9: UpdateProfile(esql, login);
		break;
		default: break;
		}
	}	

		break;
		case 9:
			updateProf = false;
		break;
		default:
			System.out.println("Invalid choice! ");
		break;
		}
	}
	}
	catch(Exception e){
		System.err.println(e.getMessage());
	}
}
   public static void searchPeople (ProfNetwork esql, String login){
      try{
	System.out.print("\tEnter a name to search for: ");
	String searchName = in.readLine();
	String query;
	query = String.format("SELECT userId, email, name, dateOfBirth FROM USR WHERE name = '%s'", searchName);
	String query2 = String.format("SELECT W.company, W.role, W.location, W.startDate, W.endDate FROM USR U, WORK_EXPR W WHERE U.name = '%s' AND U.userId = W.userId", searchName);
        String query3 = String.format("SELECT E.institutionName, E.major, E.degree, E.startdate, E.enddate FROM USR U, EDUCATIONAL_DETAILS E WHERE U.name = '%s' AND E.userId = U.userId", searchName);
	int userNum = esql.executeQuery(query);
	if(userNum > 0){
	//	String userInfo = esql.executeQuery(query);
	//	String WorkExp = esql.executeQuery(query);
	//	String EdDet = esql.executeQuery(query);

		int usrInfo = esql.executeQueryAndPrintResult(query);
		System.out.println("\n\n");
		int edInfo = esql.executeQueryAndPrintResult(query2);
		System.out.println("\n\n");
		int WorkExp = esql.executeQueryAndPrintResult(query3);
		//System.out.println("Company, role, location, start date, end date: '%s'", WorkExp);
		//System.out.println("Institution Name, major, degree, start date, end date: '%s'", EdDet);
		}
	else{
		System.out.println("There is no one with the name of " + searchName);
		System.out.println("To search for another name press 3. ");
		String tryAgain = esql.in.readLine();
		if(tryAgain == "3")
			searchPeople(esql, login);
	}
	}catch(Exception e){
		System.err.println(e.getMessage());
	}
}

 	 
}//end ProfNetwork
