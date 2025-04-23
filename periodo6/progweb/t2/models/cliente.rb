require 'active_record'

class Cliente < ActiveRecord::Base
    has_one :endereco, dependent: :destroy
    has_many :pedidos, dependent: :destroy
end