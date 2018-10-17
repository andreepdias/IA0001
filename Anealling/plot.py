import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

arquivo = 'uf250_01.cnf'
tipo_resfriamento = '8'
numero_iteracoes = 250000
numero_testes = 10

arrays = [0.0] * numero_testes

for i in range(0, numero_testes):
    arrays[i] = [0.0] * numero_iteracoes

for i in range(0, numero_testes):
    nome_arquivo = arquivo + '_' + tipo_resfriamento + '_' + str(i+1)
    with open(nome_arquivo) as f:
        j = 0
        for line in f: # read rest of lines
            arrays[i][j] = float(line)
            j = j +1

array = []

for k in range(0, numero_iteracoes):
    media = 0.0
    for i in range(0, numero_testes):
        media = media + arrays[i][k]
    media =  media / numero_testes
    array.append(media)


plt.plot(array)
plt.savefig('20_t0.png')
