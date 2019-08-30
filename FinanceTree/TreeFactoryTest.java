import java.util.Scanner;
import java.security.SecureRandom;


public class TreeFactoryTest {

	public static void main(String[] args) {
		
		Scanner input = new Scanner(System.in);
		
		// Prompt user input
				
		System.out.println("Enter number of time intervals: ");	
		int time = input.nextInt();
		while(time < 1) {
			System.out.println("Error: Please enter a positive, whole number.");
			time = input.nextInt();
		}
				
		System.out.println("Enter probability: ");	
		int probability = input.nextInt();
		while(probability < 0) {
			System.out.println("Error: Please enter a positive number.");
			probability = input.nextInt();
		}
		
		System.out.println("Enter number of iterations: ");	
		int iterations = input.nextInt();
		while(iterations < 1) {
			System.out.println("Error: Please enter a positive number.");
			iterations = input.nextInt();
		}
			
		TreeFactory t = new TreeFactory(); 
		t.create(time + 1, probability);
		Navigator n = new Navigator();
		n.navigate(iterations);
					
	}
}
