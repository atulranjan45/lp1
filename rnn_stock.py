# Step 1: Import necessary libraries
import yfinance as yf
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.preprocessing import MinMaxScaler
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import SimpleRNN, Dense

# Step 2: Load the Google stock price data using yfinance
data = yf.download('GOOGL', start='2015-01-01', end='2023-12-31')
closing_prices = data['Close'].values.reshape(-1, 1)

# Step 3: Normalize the closing prices for better performance
scaler = MinMaxScaler()
scaled_prices = scaler.fit_transform(closing_prices)

# Step 4: Create time series sequences
def create_sequences(data, window_size):
    X, y = [], []
    for i in range(len(data) - window_size):
        X.append(data[i:i + window_size])
        y.append(data[i + window_size])
    return np.array(X), np.array(y)

window_size = 60
X, y = create_sequences(scaled_prices, window_size)

# Step 5: Split the data into training and testing sets
split_index = int(len(X) * 0.8)
X_train, X_test = X[:split_index], X[split_index:]
y_train, y_test = y[:split_index], y[split_index:]

# Step 6: Build the RNN model
model = Sequential()
model.add(SimpleRNN(50, activation='tanh', input_shape=(window_size, 1)))  # RNN layer
model.add(Dense(1))  # Output layer

# Step 7: Compile the model
model.compile(optimizer='adam', loss='mse')  # Regression loss

# Step 8: Train the model
model.fit(X_train, y_train, epochs=20, batch_size=32, validation_split=0.1, verbose=1)

# Step 9: Evaluate the model and make predictions
predictions = model.predict(X_test)
predicted_prices = scaler.inverse_transform(predictions)
actual_prices = scaler.inverse_transform(y_test.reshape(-1, 1))

# Step 10: Plot predicted vs actual prices
plt.figure(figsize=(10, 6))
plt.plot(actual_prices, label='Actual Prices')
plt.plot(predicted_prices, label='Predicted Prices')
plt.title('Google Stock Price Prediction using RNN')
plt.xlabel('Time')
plt.ylabel('Price')
plt.legend()
plt.grid(True)
plt.show()
