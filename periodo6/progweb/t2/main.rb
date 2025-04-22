require 'active_record'
require 'sqlite3'

ActiveRecord::Base.establish_connection(
    adapter: 'sqlite3',
    database: 'db/delivery.sqlite3'
)

require_relative 'models/cliente'
require_relative 'models/endereco'
require_relative 'models/produto'
require_relative 'models/pedido'
require_relative 'models/item_pedido'

MODELS = {
    "clientes" => Cliente,
    "enderecos" => Endereco,
    "produtos" => Produto,
    "pedidos" => Pedido,
    "item_pedidos" => ItemPedido
}

def parse_attribute(attr)
    key, value = attr.split('=')
    value = value.gsub(/^"|"$/,'') if value
    [key.to_sym, value]
end

def parse_input(input)
    parts = input.split(/\s+(?=(?:[^"]*"[^"]*")*[^"]*$)/)
    return nil if parts.empty?

    operation = parts[0]
    table = parts[1]
    attrs = parts[2..-1]&.map { |attr| parse_attribute(attr) }&.to_h || {}

    [operation, table, attrs]
end

loop do
    print "> "
    input = gets.strip
    break if input.downcase == 'sair'
  
    operation, table, attrs = parse_input(input)
    model = MODELS[table]

    unless model
        puts "Tabela desconhecida."
        next
    end
  
    case operation
    when "insere"
        model.create(attrs)
    when "lista"
        model.all.each { |obj| puts obj.attributes }
    when "exclui"
        model.where(attrs).destroy_all
    when "altera"
        id = attrs.delete(:id)
        obj = model.find_by(id: id)
        obj.update(attrs) if obj
    else
        puts "Operação desconhecida."
    end
end