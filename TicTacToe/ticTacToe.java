package TicTacToe;
	
import javafx.application.Application;
import javafx.geometry.Pos;
import javafx.scene.Parent;
import javafx.stage.Stage;
import javafx.scene.Scene;
import javafx.scene.input.MouseButton;
import javafx.scene.layout.Pane;
import javafx.scene.layout.StackPane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;
import javafx.scene.shape.Rectangle;


public class ticTacToe extends Application {

	private int nbrRows = 3;
	private int nbrCols = 3;
	private int offset = 150;
	private Pane root = new Pane();
	private GameSq[][] board = new GameSq[nbrRows][nbrCols];
	private boolean player1 = true;
	private int width = 900;
	private int height = 900;
	private int squareDim = width/3;
		
	private Parent buildBoard() {
		
		root.setPrefSize(width, height);
		
		for(int i = 0; i < nbrRows; i++) {
			for(int j = 0; j < nbrCols; j++) {
				GameSq gameSquare = new GameSq();
				gameSquare.setTranslateX(j * squareDim);
				gameSquare.setTranslateY(i * squareDim);
				
				root.getChildren().add(gameSquare);
				
				board[j][i] = gameSquare;
			}
		}
		
		return root;
	}
	
	@Override
	public void start(Stage primaryStage) throws Exception {
		primaryStage.setScene(new Scene(buildBoard()));
		primaryStage.show();
		primaryStage.setTitle("Tic Tac Toe: O = left mouse (Pl 1); X = right mouse (Pl 2)");
	}
	
	
	private class GameSq extends StackPane {
		
		public GameSq() {
			Rectangle border = new Rectangle(squareDim, squareDim);
			border.setFill(Color.BLACK);
			border.setStroke(Color.WHITE);
					
			setAlignment(Pos.CENTER);
			getChildren().addAll(border);
			
			setOnMouseClicked(event -> {
				
				if(event.getButton() == MouseButton.PRIMARY) {
					if(!player1)
						return;
					
					drawCircle();
					player1 = false;
					
				}
				else if(event.getButton() == MouseButton.SECONDARY) {
					if(player1)
						return;
					
					drawX();
					player1 = true;
					
				}
			});
		}
		
		public double getCenterX() {
			return getTranslateX() + offset;
		}
		
		public double getCenterY() {
			return getTranslateY() + offset;
		}
		
		private void drawCircle() {
			int radius = 100;
			int radius2 = radius - 20;
			Circle cir = new Circle();
			cir.setFill(Color.BLUE);
			cir.setRadius(radius);
			cir.setLayoutX(getCenterX());
			cir.setLayoutY(getCenterY());
			root.getChildren().add(cir);
			Circle cirS = new Circle();
			cirS.setFill(Color.BLACK);
			cirS.setRadius(radius2);
			cirS.setLayoutX(getCenterX());
			cirS.setLayoutY(getCenterY());
			root.getChildren().add(cirS);
		
		}
		
		private void drawX() {
			int coordOffset = 90;
			Line line1 = new Line();
			line1.setStartX(getCenterX() - coordOffset);
			line1.setStartY(getCenterY() - coordOffset);
			line1.setEndX(getCenterX() + coordOffset);
			line1.setEndY(getCenterY() + coordOffset);
			line1.setStroke(Color.YELLOW);
			line1.setStrokeWidth(20);
			root.getChildren().add(line1);
			Line line2 = new Line();
			line2.setStartX(getCenterX() + coordOffset);
			line2.setStartY(getCenterY() - coordOffset);
			line2.setEndX(getCenterX() - coordOffset);
			line2.setEndY(getCenterY() + coordOffset);
			line2.setStroke(Color.YELLOW);
			line2.setStrokeWidth(20);
			root.getChildren().add(line2);
		}
	}
	
	public static void main(String[] args) {
		launch(args);
	}
	
}
