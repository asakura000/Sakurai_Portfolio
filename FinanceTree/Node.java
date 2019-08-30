
public class Node {
	
	// Attributes
	private Node downNode; // left child
	private Node upNode; // right child
	private int probability;
	
	// constructor
	public Node(int probability) {
		this.downNode = null;
		this.upNode = null;
		this.probability = probability;
	}
	
	// setter methods 
	public void setDownNode(Node downNode) {
		this.downNode = downNode;
	}
	
	public void setUpNode(Node upNode) {
		this.upNode = upNode;
	}
	
	public void setProbability(int probability) {
		this.probability = probability;
	}
		
	// getter methods
	public Node getDownNode() {
		return downNode;
	}
	
	public Node getUpNode() {
		return upNode;
	}
	
	public int getProbability() {
		return probability;
	}
	
}
