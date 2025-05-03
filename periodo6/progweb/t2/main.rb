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

def help
    puts <<~HELP
        ===============================================================================
        Tabelas do banco de dados:
        - #{MODELS.keys.join("\n- ")}
        ===============================================================================
        Comandos disponíveis:
        > insere <tabela> atributo1="valor1" atributo2="valor2" ...
        > exclui <tabela> id=<valor>
        > altera <tabela> id=<valor> atributo1="novo_valor" atributo2="novo_valor" ...
        > lista <tabela>
        > ajuda
        > sair
        ===============================================================================

    HELP
end

help
loop do
    print "> "
    input = gets.strip
    break if input.downcase == 'sair'
  
    operation, table, attrs = parse_input(input)
    model = MODELS[table]

    if operation == "ajuda"
        help
        next
    end

    unless model
        puts "Tabela desconhecida."
        next
    end
  
    case operation
    when "insere"
        valid_attrs = attrs.select { |key, _| model.column_names.include?(key.to_s) }
        model.create(valid_attrs)
    when "exclui"
        model.where(attrs).destroy_all
    when "altera"
        id = attrs.delete(:id)
        obj = model.find_by(id: id)
        if obj
            valid_attrs = attrs.select { |key, _| model.column_names.include?(key.to_s) }
            obj.update(valid_attrs)
        end
    when "lista"
        model.all.each { |obj| puts obj.attributes }
    else
        puts "Operação desconhecida."
    end
    puts
end
