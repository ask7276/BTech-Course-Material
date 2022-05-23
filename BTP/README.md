# BTP

This is my project work for the BTech Thesis Project in IIT Guwahati. In this, I introduced a new hyper - parameter ( β ) in subgraph sampling process in GraphBert (paper[https://arxiv.org/abs/2001.05140]) and investigated the results on cora and citeseer dataset.

Significance of parameter β: For higher subgraph size k, we have higher β (close to 1 ) performing better while for smaller k, smaller β is preferred. So for higher k, we do not need to consider attributes specifically. But instead nodes having similar attributes develop similar neighborhoods and they get considered indirectly by neighborhood similarity while we make β = 1. 

The detailed report can be found as AI_ML-180101037.pdf

