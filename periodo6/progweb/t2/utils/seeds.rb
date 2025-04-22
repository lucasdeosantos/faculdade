require 'active_record'
require 'sqlite3'

ActiveRecord::Base.establish_connection(
    adapter: 'sqlite3',
    database: 'db/delivery.sqlite3'
)

require_relative '../models/cliente'
require_relative '../models/endereco'
require_relative '../models/produto'
require_relative '../models/pedido'
require_relative '../models/item_pedido'

10.times do |i|
    Cliente.create(nome: "Cliente #{i+1}", telefone: "99999#{i+1000}")
    Produto.create(descricao: "Produto #{i+1}", preco: rand(10..100))
    Pedido.create(cliente_id: i+1)
    Endereco.create(cliente_id: i+1, rua: "Rua #{i+1}", cidade: "Cidade #{i+1}")
    ItemPedido.create(pedido_id: i+1, produto_id: i+1, quantidade: rand(1..5))
end