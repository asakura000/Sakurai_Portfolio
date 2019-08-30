export class Board {
  constructor(numberOfRows, numberOfColumns, numberOfBombs) {
    this._numberOfBombs = numberOfBombs;
    this._numberOfTiles = numberOfRows * numberOfColumns;
    this._playerBoard = Board.generatePlayerBoard(numberOfRows, numberOfColumns);
    this._bombBoard =  Board.generateBombBoard(numberOfRows, numberOfColumns, numberOfBombs);
  }

  get playerBoard(){
    return this._playerBoard;
  }

  flipTile(rowIndex, columnIndex) {
    if(this._playerBoard[rowIndex][columnIndex] !== ' '){
      console.log('This tile has already been flipped!');
      return;
    }
    else if(this._bombBoard[rowIndex][columnIndex] === 'B'){
      this._playerBoard[rowIndex][columnIndex] = 'B';
    }
    else {
      this._playerBoard[rowIndex][columnIndex] = this.getNumberOfNeighborBombs(rowIndex, columnIndex);
    }
    this._numberOfTiles--;
  }

  getNumberOfNeighborBombs(rowIndex, columnIndex) {
  //the purpose of this function is to return the number of bombs in an adjacent neighbor.

    const neighborOffsets = [
      [-1, -1],
      [-1, 0],
      [-1, 1],
      [0, -1],
      [0, 1],
      [1, -1],
      [1, 0],
      [1, 1]
    ];

    const numberOfRows = this._bombBoard.length;
    const numberOfColumns = this._bombBoard[0].length;

    let numberOfBombs = 0;

    neighborOffsets.forEach(offset => {
      const neighborRowIndex = rowIndex + offset[0];
      const neighborColumnIndex = columnIndex + offset[1];
      if(neighborRowIndex >= 0 && neighborRowIndex < numberOfRows && neighborColumnIndex >= 0 && neighborColumnIndex < numberOfColumns){
        if(this._bombBoard[neighborRowIndex][neighborColumnIndex] === 'B'){
          numberOfBombs++;
        }
      }
    })
    return numberOfBombs;
  }

  hasSafeTiles() {
    return this._numberOfTiles !== this._numberOfBombs;
  }

  print(){
    console.log(this.playerBoard.map(row => row.join(' | ')).join('\n'));
    /*
    This updated printBoard() function will accept a game board as a parameter, iterate through each of its rows, join the individual elements in each row, and then join all rows together. It will return a brand new game board as a single string to be easily printed.
    */
  }

  static generatePlayerBoard(numberOfRows, numberOfColumns) {
    const board = [];

    for(let rowIndex = 0; rowIndex < numberOfRows; rowIndex++){
      const row = [];
      for(let columnIndex = 0; columnIndex < numberOfColumns; columnIndex++){
        row.push(' ');
      }
      board.push(row);
    }
    return board;
  }

  static generateBombBoard(numberOfRows, numberOfColumns, numberOfBombs) {
    const board = [];

    for(let rowIndex = 0; rowIndex < numberOfRows; rowIndex++){
      const row = [];
      for(let columnIndex = 0; columnIndex < numberOfColumns; columnIndex++){
        row.push(null);
      }
      board.push(row);
    }

    let numberOfBombsPlaced = 0; //Bomb counter

    while(numberOfBombsPlaced < numberOfBombs){
      let ramdomRowIndex = Math.floor(Math.random() * numberOfRows);
      let ramdomColIndex = Math.floor(Math.random() * numberOfColumns);
      if(board[ramdomRowIndex][ramdomColIndex] !== 'B'){
        board[ramdomRowIndex][ramdomColIndex] = 'B';
        numberOfBombsPlaced ++;
      }
    }
    return board;
  }
}
