require 'active_record'

class Cliente < ActiveRecord::Base
    has_one :endereco
    has_many :pedido
end