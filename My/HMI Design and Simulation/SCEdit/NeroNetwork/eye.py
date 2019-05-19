import Nero
from numpy import array

if __name__ == "__main__":
    eye_cells=Nero.NeuralNetwork();
    print("随机的初始突触权重：")
    print(eye_cells.synaptic_weights)
    # 训练集。四个样本，每个有3个输入和1个输出
    training_set_inputs = array([[0, 0, 1], [1, 1, 1], [1, 0, 1], [0, 1, 1]])
    training_set_outputs = array([[0, 1, 1, 0]]).T
    # 用训练集训练神经网络
    # 重复一万次，每次做微小的调整
    eye_cells.train(training_set_inputs, training_set_outputs, 10000)
    print("训练后的突触权重：")
    print(eye_cells.synaptic_weights)
    # 用新数据测试神经网络
    print("考虑新的形势 [1, 0, 0] -> ?: ")
    print(eye_cells.think(array([1, 0, 0])))
