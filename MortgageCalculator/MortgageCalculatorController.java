
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Slider;

import javafx.scene.control.TextField;
import java.math.BigDecimal;
import java.math.RoundingMode;

import java.text.NumberFormat;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;


public class MortgageCalculatorController {
	
	// formatters for currency and percentages
	
	private static final NumberFormat currency = NumberFormat.getCurrencyInstance();
	private static final NumberFormat percent = NumberFormat.getPercentInstance();
	
	private BigDecimal customLoanDuration;
	
	BigDecimal calculateMonthly(int years, BigDecimal principal, BigDecimal interestRate) {
	
		/*
		Use equation M = P * [r(1 + r)^n] / [(1+r)^n - 1]
		where M = monthly payment
		P (principal) = purchase price - down payment
		r (rate)  = monthly interest rate which is annual interest rate / 12
		n = number of payments (number of years of the loan * 12)
		*/
		
		int n = years * 12;
		
		// 1) Convert Big Decimal arguments to float for easier calculation
		
		float p = principal.setScale(2, RoundingMode.DOWN).floatValue();
		float rate = interestRate.setScale(2, RoundingMode.DOWN).floatValue();
		
		float r = rate / (12 * 100); // monthly interest rate 
		float x = (float) Math.pow(1 + r, n); // (1 + r)^n expressed as a single variable 
		float m = p*(r*x)/(x-1);
		
		// 2) Convert float back to Big Decimal
		
		BigDecimal monthlyPayment = BigDecimal.valueOf(m);
		
		return monthlyPayment;
	}

	@FXML
    private TextField purchasePriceTextField;

    @FXML
    private TextField downPaymentTextField;

    @FXML
    private TextField monthlyPaymentCustomTextField;

    @FXML
    private TextField interestRateTextField;

    @FXML
    private Slider loanDurationSlider;

    @FXML
    private TextField loanAmountTextField;

    @FXML
    private TextField monthlyPayment10YrTextField;

    @FXML
    private TextField monthlyPayment20YrTextField;
    
    @FXML
    private TextField monthlyPayment30YrTextField;
    
    @FXML
    private TextField customLoanDurationTextField;
    
    public void initialize() {
    	
    	currency.setRoundingMode(RoundingMode.HALF_UP);
    	
    	loanDurationSlider.valueProperty().addListener(
    		new ChangeListener<Number>() {
    			public void changed(ObservableValue<? extends Number> ov, Number oldValue, Number newValue) {
    				customLoanDuration = BigDecimal.valueOf(newValue.intValue());
    				customLoanDurationTextField.setText(customLoanDuration.toString());
    			}
    		}
    			  			
    	);
    	  	
    }
    

    @FXML
    void calculateButtonPressed(ActionEvent event) {
    	
    	try {
    		
    		BigDecimal purchasePrice = new BigDecimal(purchasePriceTextField.getText());
    		BigDecimal downPayment = new BigDecimal(downPaymentTextField.getText());
    		BigDecimal interestRate = new BigDecimal(interestRateTextField.getText());
    				
    		BigDecimal principal = purchasePrice.subtract(downPayment);
    		loanAmountTextField.setText(currency.format(principal));
    		
    		int nbr10Yr = 10;
    		BigDecimal monthly10 = calculateMonthly(nbr10Yr, principal,interestRate);
    		monthlyPayment10YrTextField.setText(currency.format(monthly10));
    		
    		int nbr20Yr = 20;
    		BigDecimal monthly20 = calculateMonthly(nbr20Yr, principal,interestRate);
    		monthlyPayment20YrTextField.setText(currency.format(monthly20));
    		
    		int nbr30Yr = 30;
    		BigDecimal monthly30 = calculateMonthly(nbr30Yr, principal,interestRate);
    		monthlyPayment30YrTextField.setText(currency.format(monthly30));
    		
    	    		
    		int nbrCustom = customLoanDuration.intValue(); 
    		BigDecimal monthlyCustom = calculateMonthly(nbrCustom, principal,interestRate);
    		monthlyPaymentCustomTextField.setText(currency.format(monthlyCustom));
    		
  		
    	}
    	catch(NumberFormatException ex) {
    		purchasePriceTextField.setText("Enter purchase price");
    		purchasePriceTextField.selectAll();
    		purchasePriceTextField.requestFocus();
    	}
    	catch(NullPointerException e) {
    		System.out.print("Caught Null Pointer Exception"); 
    	}
    		
    		
    }
     
}

