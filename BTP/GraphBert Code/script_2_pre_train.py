import numpy as np
import torch

from code.DatasetLoader import DatasetLoader
from code.MethodBertComp import GraphBertConfig
from code.MethodGraphBertNodeConstruct import MethodGraphBertNodeConstruct
from code.MethodGraphBertGraphRecovery import MethodGraphBertGraphRecovery
from code.ResultSaving import ResultSaving
from code.Settings import Settings

#---- 'cora' , 'citeseer', 'pubmed' ----

dataset_name = 'cora'

np.random.seed(1)
torch.manual_seed(1)

#---- cora-small is for debuging only ----
if dataset_name == 'cora-small':
    nclass = 7
    nfeature = 1433
    ngraph = 10
elif dataset_name == 'cora':
    nclass = 7
    nfeature = 1433
    ngraph = 2708
elif dataset_name == 'citeseer':
    nclass = 6
    nfeature = 3703
    ngraph = 3312
elif dataset_name == 'pubmed':
    nclass = 3
    nfeature = 500
    ngraph = 19717



#---- Pre-Training Task #1: Graph Bert Node Attribute Reconstruction (Cora, Citeseer, and Pubmed) ----
if 0:
    #---- hyper-parameters ----
    if dataset_name == 'pubmed':
        lr = 0.001
        max_epoch = 200 # ---- do an early stop when necessary ----
    elif dataset_name == 'cora':
        lr = 0.001
        max_epoch = 200 # ---- do an early stop when necessary ----
    elif dataset_name == 'citeseer':
        lr = 0.002
        max_epoch = 2500 # it takes a long epochs to converge, probably more than 2000

    x_size = nfeature
    hidden_size = intermediate_size = 32
    num_attention_heads = 2
    num_hidden_layers = 2
    y_size = nclass
    graph_size = ngraph
    residual_type = 'graph_raw'
    # --------------------------

    for k in [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 25, 30, 35, 40, 45, 50, 60, 70]:
        for alpha in [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1]:
            print('************ Start ************')
            print('GrapBert, dataset: ' + dataset_name + ', Pre-training, Node Attribute Reconstruction.' + ', k: ' + str(k) + ', alpha:' + str(alpha))
            # ---- objection initialization setction ---------------
            data_obj = DatasetLoader()
            data_obj.dataset_source_folder_path = './data/' + dataset_name + '/'
            data_obj.dataset_name = dataset_name
            data_obj.k = k
            data_obj.load_all_tag = True
            data_obj.alpha = alpha

            bert_config = GraphBertConfig(residual_type = residual_type, k=k, x_size=nfeature, y_size=y_size, hidden_size=hidden_size, intermediate_size=intermediate_size, num_attention_heads=num_attention_heads, num_hidden_layers=num_hidden_layers)
            method_obj = MethodGraphBertNodeConstruct(bert_config)
            method_obj.max_epoch = max_epoch
            method_obj.lr = lr

            result_obj = ResultSaving()
            result_obj.result_destination_folder_path = './result/GraphBert/'
            result_obj.result_destination_file_name = dataset_name + '_' + str(k) + '_' + str(alpha) + '_node_reconstruction'

            setting_obj = Settings()

            evaluate_obj = None
            # ------------------------------------------------------

            # ---- running section ---------------------------------
            setting_obj.prepare(data_obj, method_obj, result_obj, evaluate_obj)
            setting_obj.load_run_save_evaluate()
            # ------------------------------------------------------
            torch.save(method_obj.state_dict(), './result/PreTrained_GraphBert/cora/model_' + str(k) + '_' + str(alpha))
            print('************ Finish ************')
#------------------------------------







#---- Pre-Training Task #2: Graph Bert Network Structure Recovery (Cora, Citeseer, and Pubmed) ----
if 0:
    #---- hyper-parameters ----
    if dataset_name == 'pubmed':
        lr = 0.001
        max_epoch = 200 # ---- do an early stop when necessary ----
    elif dataset_name == 'cora':
        lr = 0.001
        max_epoch = 200 # ---- do an early stop when necessary ----
    elif dataset_name == 'citeseer':
        lr = 0.005
        max_epoch = 2500 # it takes a long epochs to converge, probably more than 2000
    else:
        lr = 0.01
        max_epoch = 200

    x_size = nfeature
    hidden_size = intermediate_size = 32
    num_attention_heads = 2
    num_hidden_layers = 2
    y_size = nclass
    graph_size = ngraph
    residual_type = 'graph_raw'
    # --------------------------

    for k in [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 25, 30, 35, 40, 45, 50, 60, 70]:
        for alpha in [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1]:
            print('************ Start ************')
            print('GrapBert, dataset: ' + dataset_name + ', Pre-training, Graph Structure Recovery.' + ', k: ' + str(k) + ', alpha:' + str(alpha))
            # ---- objection initialization setction ---------------
            data_obj = DatasetLoader()
            data_obj.dataset_source_folder_path = './data/' + dataset_name + '/'
            data_obj.dataset_name = dataset_name
            data_obj.k = k
            data_obj.load_all_tag = True
            data_obj.alpha = alpha

            bert_config = GraphBertConfig(residual_type = residual_type, k=k, x_size=nfeature, y_size=y_size, hidden_size=hidden_size, intermediate_size=intermediate_size, num_attention_heads=num_attention_heads, num_hidden_layers=num_hidden_layers)
            method_obj = MethodGraphBertGraphRecovery(bert_config)
            method_obj.load_state_dict(torch.load('./result/PreTrained_GraphBert/cora/model_' + str(k) + '_' + str(alpha)), strict=False)
            method_obj.max_epoch = max_epoch
            method_obj.lr = lr

            result_obj = ResultSaving()
            result_obj.result_destination_folder_path = './result/GraphBert/'
            result_obj.result_destination_file_name = dataset_name + '_' + str(k) + '_' + str(alpha) + '_graph_recovery'

            setting_obj = Settings()

            evaluate_obj = None
            # ------------------------------------------------------

            # ---- running section ---------------------------------
            setting_obj.prepare(data_obj, method_obj, result_obj, evaluate_obj)
            setting_obj.load_run_save_evaluate()
            # ------------------------------------------------------
            torch.save(method_obj.state_dict(), './result/PreTrained_GraphBert/cora/model_' + str(k) + '_' + str(alpha))
            print('************ Finish ************')
#------------------------------------

