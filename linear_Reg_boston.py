# Step 1: Import necessary libraries
# import numpy as np
# import tensorflow as tf
from tensorflow.keras.datasets import boston_housing
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from sklearn.preprocessing import StandardScaler

# Step 2: Load the Boston Housing dataset from TensorFlow
(x_train, y_train), (x_test, y_test) = boston_housing.load_data()

# Step 3: Scale the features for better performance
scaler = StandardScaler()
x_train = scaler.fit_transform(x_train)
x_test = scaler.transform(x_test)

# Step 4: Build the Deep Neural Network model
model = Sequential()
model.add(Dense(64, activation='relu', input_shape=(x_train.shape[1],)))  # 1st hidden layer
model.add(Dense(64, activation='relu'))                                   # 2nd hidden layer
model.add(Dense(1))  # Output layer (no activation, for regression)

# Step 5: Compile the model
model.compile(optimizer='adam', loss='mse', metrics=['mae']) #must compile before training else error


# Step 6: Train the model
model.fit(x_train, y_train, epochs=100, batch_size=16, validation_split=0.1, verbose=1)

# Step 7: Evaluate the model
loss, mae = model.evaluate(x_test, y_test)
print(f"\nTest Mean Absolute Error: {mae:.2f}")

# Step 8: Predict and show comparison
predictions = model.predict(x_test[:5])
print("\nPredicted vs Actual house prices:")
for i in range(5):
    print(f"Predicted: ${predictions[i][0]*1000:.2f}, Actual: ${y_test[i]*1000:.2f}")
