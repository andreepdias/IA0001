import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

plt.style.use('classic')

arquivo = 'uf250_01.cnf'
tipo_resfriamento = '8'
numero_iteracoes = 250000
numero_testes = 10
prefixo = ''

arrays = [0.0] * numero_testes
for i in range(0, numero_testes):
    arrays[i] = [0.0] * numero_iteracoes

for i in range(0, numero_testes):
    nome_arquivo = arquivo + '_' + tipo_resfriamento + '_' + str(i+1) + prefixo
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
    print(media)
    array.append(media)


# print(arrays[0][0])
# print(arrays[1][0])
# print(arrays[2][0])
# print(arrays[3][0])
# print(arrays[4][0])
# print(arrays[5][0])
# print(arrays[6][0])
# print(arrays[7][0])
# print(arrays[8][0])
# print(arrays[9][0])
# print(array[0])

plt.plot(array, marker=None)
plt.savefig('250_aneling.png')
