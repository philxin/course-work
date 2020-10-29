from scipy.optimize import minimize
from sklearn.datasets import make_regression
from sklearn.linear_model import Ridge
import numpy as np


def ridge_cost_function(beta, x, y, lam):
    y_hat = [sum(x[i,:]@beta.reshape(-1,1)) for i in range(x.shape[0])] # compute y_hat
    return sum((y[:] - y_hat[:])**2) + lam*sum(beta[:]**2) # ridge's cost function

def least_squares_reg(x,y):
    return np.linalg.solve(x.T@x, x.T@y)


def ridge_regression(x, y, lambda_):  # lambda_: hyperparameter for penalty
    ones = np.ones((x.shape[0],1))
    x = np.hstack((ones,x))
    
    beta_init = least_squares_reg(x,y) # # initial guess of betas
    result = minimize(ridge_cost_function, beta_init, method='nelder-mead',
                     args=(x, y, lambda_))
    return result.x # return betas: [ intercept, beta_1, beta_2, ..., beta_n ]


# code for testing
"""
X, y, coef = make_regression(n_samples=1000, n_features=6, noise=0.1, coef=True)

print("beta from make_regression(): " ,coef,'\n')
print("beta from my ridge regression: ", ridge_regression(X, y , lambda_=10), '\n')

reg = Ridge(alpha=10)
reg.fit(X, y)
print("beta from sklearn ridge regression: ", reg.intercept_, reg.coef_)
"""