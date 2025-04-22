require 'active_record'

class Produto < ActiveRecord::Base
    has_many :item_pedidos
    has_many :pedidos, through: :item_pedidos
end