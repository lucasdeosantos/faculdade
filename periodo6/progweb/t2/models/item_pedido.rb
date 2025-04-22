require 'active_record'

class ItemPedido < ActiveRecord::Base
    belongs_to :pedido
    belongs_to :produto
end