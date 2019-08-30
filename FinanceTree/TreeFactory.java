import java.security.SecureRandom;

public final class TreeFactory { 
	
	// Attributes
	
	static Node ROOT;
	
	// Method 
	
	public static Node create(int time, int probability) {
					
		Node currentNode = new Node(probability);
		
		// Base case
		
		if(time == 0) {
			return null;		
		}
		if(ROOT == null) {
			ROOT = currentNode;
		}
		
		// each node throughout the tree will hold a key value of probability. 
		
		currentNode.setDownNode(create(time - 1, probability)); 
				
		currentNode.setUpNode(create(time - 1, probability));
				
		return currentNode;
		
	}
		 
}
