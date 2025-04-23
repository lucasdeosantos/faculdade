require 'active_record'

class Pedido < ActiveRecord::Base
    belongs_to :cliente
    has_many :item_pedidos, dependent: :destroy
    has_many :produtos, through: :item_pedidos
end