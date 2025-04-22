require 'active_record'
require 'sqlite3'

Dir.mkdir("db") unless Dir.exist?("db")

ActiveRecord::Base.establish_connection(
    adapter: 'sqlite3',
    database: 'db/delivery.sqlite3'
)

ActiveRecord::Base.connection.create_table :clientes do |t|
    t.string :nome
    t.string :telefone
end

ActiveRecord::Base.connection.create_table :enderecos do |t|
    t.string :rua
    t.string :cidade
    t.references :cliente, foreign_key: true
end

ActiveRecord::Base.connection.create_table :produtos do |t|
    t.string :descricao
    t.decimal :preco, precision: 10, scale: 2
end

ActiveRecord::Base.connection.create_table :pedidos do |t|
    t.references :cliente, foreign_key: true
    t.datetime :data_pedido, default: -> { 'CURRENT_TIMESTAMP' }
end

ActiveRecord::Base.connection.create_table :item_pedidos do |t|
    t.references :pedido, foreign_key: true
    t.references :produto, foreign_key: true
    t.integer :quantidade
end