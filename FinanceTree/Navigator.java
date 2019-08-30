import java.security.SecureRandom;

public class Navigator {

	// Attributes
	
	private TreeFactory tree;
	private int iterations;
	
	// Method
	
	public void navigate(int iterations) {

		Node current;
		Node root = tree.ROOT;
	      		  		
	    SecureRandom randomProb = new SecureRandom();
	    		
	    int randomNbr; 
	    	
	    int probability = root.getProbability();
	    	
	    current = root; // start cursor at root of tree
	    	   		   		
		for(int counter = 1; counter <= iterations; counter ++) {
			System.out.println();
			System.out.println("Iteration number: " + counter);
			System.out.println("Probability: " + probability);
			System.out.println();
			current = root; // bring cursor back to root node 
				
			while(current.getDownNode() != null || current.getUpNode() != null) {
	
				randomNbr = 1 + randomProb.nextInt(100);
				System.out.println("Generated Number is " + randomNbr);
				if(randomNbr <= probability) {
					System.out.println("UP " + randomNbr + "%");
					current = current.getUpNode();	// traverse to right child				
				}
				else {		
					int downProb = 100 - randomNbr;
					System.out.println("DOWN " + downProb + "%");	
					current = current.getDownNode();  // traverse to left child				
				}
			}   		
		}
	}
}
