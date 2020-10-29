
"""
Implementation of Newey-West corrected OLS estimator with autocovariance term
"""

import numpy as np
import pandas as pd


class linear_model:
    def __init__(self,x,y):
        self.x = x
        self.y = y
        self.b = np.linalg.solve(x.T@x,x.T@y)
        self.e = y - x@self.b
        self.vb = self.vcov_b()
        self.se = np.sqrt(np.diagonal(self.vb))
        self.tstat = self.b/self.se
    def vcov_b(self):
        x = self.x
        e = self.e
        return e.var()*np.linalg.inv(x.T@x)


class newey_west(linear_model):
    def vcov_b(self):
        x = self.x
        e = self.e
        
        bread = np.linalg.inv(x.T@x)@x.T
        
        ee = 0.0
        for i in range(len(e.values)-1):
            ee += e.values[i]*e.values[i+1]
            
        sigma_1 = round(ee/(len(e.values)-1), 6)
        
        n_se = bread.shape[1] # get the dimension to create meat
        meat = np.zeros((n_se, n_se)) # create a square matrix
        sigma_sq = round(e.var() , 6)

        # assign values to the meat matrix
        for i in range(n_se):
            meat[i, i] = sigma_sq
            if i != 0 and i != n_se-1:
                meat[i, i-1] = sigma_1
                meat[i, i+1] = sigma_1
            elif i == 0:
                meat[i, i+1] = sigma_1
            elif i == n_se - 1:
                meat[i, i-1] = sigma_1

        sandwich = bread@meat@bread.T
        #print(meat)
        return sandwich
